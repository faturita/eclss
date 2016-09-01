/******************************************************************************
 * email.cpp
 *
 * Connection Handlers.
 *
 * Version 1.0.0 (October 17 2008)
 *
 * THIS CODE IS FURNISHED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dlfcn.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timeb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/ioctl.h>
#include <fcntl.h>

#include <errno.h>
#include <time.h>
#include <unistd.h>

#include "connection.h"

/**
 * Check if the URL is working properly.
 * 
 *
 * @param   host    
 * @param   port    
 * @param   msg     Message to send.
 * @param   expmsg  Message to Receive.
 * @return  0 OK, 1 ERROR.
 */
int checkConnection(char *host, char *port, char *msg, char *expectedMsg)
{
    int sd;
    int ret=1;
    struct sockaddr_in sa ;

    int iTimeout = 20000;
    struct hostent *hp;
    int val;
    int flags;

    if ( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        printf ("Socket Creation Error.\n");
        return -1;
    }

    memset(&sa, '\0', sizeof(sa));

    sa.sin_family       = AF_INET;
    sa.sin_addr.s_addr        = inet_addr(host);
    sa.sin_port         = htons( atoi(port) );

    if ( (connect(sd, (struct sockaddr*)&sa, sizeof(sa))) < 0 ) {
        printf ("Connection Error\n");
        return -1;
    }

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

    printf ("Connect to %s:%s successfully\n", host, port);

	char buf[2048];

    sprintf(buf,"%s\r\n",msg);

    printf ("Sent: %s\n", buf);

	val = write(sd, buf, strlen(buf));

    if (val < 0) {
        printf ("Broken pipe!");
        return 1;
    }


    struct timeb tm,tm2;

    ftime(&tm);

    while ( ( (val = read( sd, buf, 4096))!=0 )  )
    {
    
	    if (val > 0 ) {
	        buf[val] = '\0';

            printf ("[%s]",buf);

            if (strncmp(buf,expectedMsg,strlen(expectedMsg))==0) 
                ret = 0;
            else 
                ret = 1;

            break;
	    
        }

        // HINT:  here sleep(0) could work.

        // Get timestamp.

        ftime(&tm2);

        int mill = (tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm);

        // HINT: It will be good if this check could be done by configuration
        // in order to allow also (strcmp(resultcode,"000")!=0)

        if ( mill > iTimeout ) {
            printf ("Giving up after %d milliseconds\n", iTimeout);
            // Let's wait for 10 s and if the connection is not closed by the other side
            // then I do it here. 
            // TODO: Take the Keep-Alive tag and use the amount of seconds there to change
            // the max wait time here.

            break;
        }
    }

    close(sd);

    return ret;

}


