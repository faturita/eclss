/* ========================================================================= 
 *                                                                           
 *       ECLSS (Environmental Control and Life Support System)               
 *                                                                           
 * ========================================================================= 
 * 
 * System Monitor Application
 *
 * Starting Date: 20/03/2005
 * Author:  $Author: faturita $
 * Version: $Revision: 1.9 $
 *
 *
 * The process start as a demon server, runs a connection socket for incoming 
 * connections and a thread to handle the status of the different monitored
 * servers.
 * 
 *
 * With every new established connection, a new thread is generated.
 *
 * The Control Thread runs every 40 seconds.  Reads the eclss.xml file the first
 * time or when the pointer to the structure is null.  Check each one of the URLs 
 * detailed in the XML file and update the information of the XML with the result
 * of the attempted connection.  The XML structure in memory contains the entire
 * information about the configuration, and also, the results of the checking.
 * If some problem is detected an email is composed and sended to the email 
 * addressed also configured in the XML file.  The email is sent only the first
 * time a change has occurred.
 * 
 * Each execution thread that is handling the incoming http request, take the XML
 * structure in memory (in case it is NULL, the information is readed from the file) 
 * and process it with the corresponding XSL file. The output, in HTML form, is sent 
 * to the client.
 * 
 * There are also different commands, based on the requested URL directory.
 * 
 * KNOWN ISSUES:
 * 
 * -Somewhere in this code, or inside the libraries of XML/XSL there is an specific
 * condition that hungs the application completely.  This is very rare but happens after
 * the application has been running for around 1 month.
 * -The Server Socket Release of resources is wrong somewhere.  This is really annoying
 * because one the program is stopped, the server cannot be used quickly (the port is in 
 * use).  This could be related with TCP protocol timming but anywhere the OS must known 
 * that the server is completely down.  Maybe the order of the system calls is wrong 
 * somewhere.
 * 
 *
 * faturita ECLSS1-0-1  11/07/2005
 * Proxy Authentication Included.
 *
 * faturita ECLSS1-0-2  21/09/2005
 * It is possible now to send POSTDATA in each request and customized queries can be used
 * to search for certain patterns inside the application.  Also checks for certain watermarks
 * like "Error in the application" or "Exception in" very common in unexpected errors.
 * (Check XML configuration)
 *
 * faturita ECLSS1-0-3  29/10/2005
 * SIGPIPE Error handling
 *
 * ========================================================================= 
 *                                                                           
 * ========================================================================= 
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <dlfcn.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <fcntl.h>

#include <errno.h>
#include <signal.h>
#include <syslog.h>


#define	FILE_MODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

#include "https.h"
#include "handler.h"

// Flag to detect if the main process must stop execution or not.
int shouldistop;
int msd;

int DEBUG=1;

// Seed random generation algorithm
void init_random()
{
    time_t tim;
    time(&tim);
    srand((unsigned int)(tim));
}

/**
 * Initializing server socket structure for fake web server.
 * 
 */ 
int w_init(char *port) 
{
    int sd;
    struct sockaddr_in sa ;
   
    struct hostent *hp;
    
    int len, flags;
    
    if ( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    	printf ("Could not allocate socket.\n");
	    return -1;
	}
    
    memset(&sa, '\0', sizeof(sa));
    
    sa.sin_family 	= AF_INET;
    sa.sin_addr.s_addr	= INADDR_ANY;
    sa.sin_port		= htons( atoi(port) );
    

    if ( bind(sd, (struct sockaddr*)&sa, sizeof(sa))  < 0 ) {
        printf ("Cannot bind socket to specified port (hint: is the port already in use?)\n");
        exit(1);
    }

    if ( listen(sd,10) < 0 ) {
        printf ("Cannot set backlog size.\n");
        exit(1);
    }

    return sd;

}

int w_accept(int sd) 
{
    struct sockaddr_in cli_addr;
    int flags;

    unsigned int lencli_addr = sizeof(cli_addr);

    if ( (flags = fcntl(sd, F_GETFL, 0)) < 0) {
	    printf ("Error fcntl get.");
    } else {
	    flags |= O_NONBLOCK;
	    if (fcntl(sd, F_SETFL, flags) < 0) {
    	    printf ("Error fcntl set");
        }  else {
	        //printf ("Setting NONBLOCKING IO");
        }
    }
    int ads;

    while ( shouldistop!=1 ) {
        if ( (ads = accept(sd, 0, 0)) < 0 ) {
            if (errno != EAGAIN) {
                printf ("Remote Connection ERROR.");
                return -1;
            }
        } else {
            return ads;
        }
    }

    printf ("Stop executing....");
    return -1;
}

static void sig_pipe(int signo)
{
    printf ("Closing pipe....\n");
}

/**
 * Signal Interrupt Handler.
 * -2 SIG_INT
 *
 * Clean up the server socket.
 *
 **/
static void sig_interrupt(int signo) {

    close(msd);

    printf ("Shutting off\r\n");

    xmlMemoryDump();
    
    exit(0);
}

/**
 * Main del Server.
 *
 * @param   argv[1]     Port
 * @param   -v          Verbose TRUE
 *
 **/
int main(int argc, char *argv[]) {
    int val = 0;
    int sd;
    char inbuf[4096];
    char buf[4096];
    pid_t pid;
    struct sched_param param;

    //xmlDocPtr doc=NULL;

    //updateStatus(doc);

    //exit(1);
    
    if (argc < 2) {
        fprintf (stderr, "%s [port]\r\n",argv[0]);

        fprintf (stderr, "\nCheck using http://hostname:[port]/\r\n");
        fprintf (stderr, "Logging /var/log/messages\n");
        fprintf (stderr, "Shutdown the daemon using SIGINT to the parent process or http://hostname:[port]/stop\r\n");
        fprintf (stderr, "Configuration file reloading: http://hostname:[port]/reload\r\n");
        exit(1);
    }

    if ((argc >= 3) && strcmp(argv[2],"-v")==0 )
        DEBUG=1;

    // ( Oni o ikiru ! )

    // Unbind from the parent process and start in background (standard procedure)
    if ( (pid = fork()) < 0 ) {
        fprintf (stderr,"Fork Error (Hint: no more processes left)\n");
        return -1;
    }

    if ( pid != 0 ) {
        return 0;
    }

    // Not a process header
    setsid();

    // Overwrite properties on files
    umask(0);

    // Signal handler Ctrl+C
    signal(SIGINT, sig_interrupt);

    // Signal handler for SigPIPE
    signal(SIGPIPE, sig_pipe);

    // Random initializing
    init_random();

    shouldistop=0;

    // xml starting
    xmlInitParser();

    // Openning log (dumps in /var/log/messages)
    openlog(argv[0],LOG_INFO,LOG_USER);

    syslog(LOG_INFO, "ECLS Starting on port %s\n", argv[1]);

    // Init the socket server descriptor.
    msd = w_init(argv[1]);

    if (msd == -1) {
        fprintf (stderr,"Server starting error, try again.\r\n");
        exit(-1);
    }
    
    pthread_t updatethread;

    // Start the connection check thread.
    pthread_attr_t  tattr;
    pthread_attr_init(&tattr);
    
    // Note:  The schedulling set does not work properly in every distribution.
    //pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
    //param.sched_priority = 10;
    //pthread_setschedparam(updatethread, SCHED_RR, &param);

    pthread_create(&updatethread, &tattr, &update_thread, NULL);
  	
    pthread_t th;
    while (1) {

        if ( (sd = w_accept(msd)) < 0) {
            break;
        }

        pthread_attr_t  attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

        // @NOTE: If you create the thread too quickly, the sd value is replaced by accept when two connections arrive simultaneously.
        //        and that means that one socket identifier is lost and the client will hang.
        pthread_create(&th, &attr, &request_handler, (void*)&sd);
        usleep(3000);                                                   // @NOTE: Wait to avoid loosing parameters inside the thread.

        
        //if ( fork() == 0) {
        //    request_handler(&sd);
        //    close(sd);
        //    exit(0);
        //} else {
        //    close (sd);
        //}
            
            
        //pthread_join( th, NULL);
        //request_handler(&sd);

    }

    shutdown(msd,SHUT_RDWR);

    close(msd);

    printf ("Shutting off\r\n");

    xmlMemoryDump();
    
    exit(0);
}

