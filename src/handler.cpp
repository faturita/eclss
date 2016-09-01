/******************************************************************************
 * handler.cpp
 *
 * Super Handler of Incoming Connections.
 *
 * Monitor: Monitor here means this server.
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
#include <sys/time.h>
#include <sys/timeb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <fcntl.h>

#include <errno.h>
#include <signal.h>
#include <syslog.h>


// Application includes
#include "https.h"
#include "email.h"
#include "connection.h"
#include "handler.h"
#include "base64.h"

// Static Public doc with the XML tree with the config information and the status info
static xmlDocPtr doc=NULL;

// When the program has started.  Purpose: check uptime of the monitor server.
static long currentTimeMillisStarted=0;

// Semaphore for accessing the XML structure.
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

// Runtime Debug flag
extern int DEBUG;

// Stopping flag (ShouldIStop?)
extern int shouldistop;

/** 
 * Wait function for threads.
 * (seconds) y go on.
 *
 * @param   seconds     Waiting seconds.
 */
void pthread_wait(int seconds) {
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    struct timeval now;
    struct timezone ts;
    struct timespec timeout;
    int retcode;

    pthread_mutex_lock(&mutex1);
    gettimeofday(&now,&ts);
    timeout.tv_sec = now.tv_sec + seconds;
    timeout.tv_nsec = now.tv_usec * 1000;
    retcode = 0;
    while (retcode != ETIMEDOUT) {
        retcode = pthread_cond_timedwait(&cond, &mutex1, &timeout);
    }
    if (retcode == ETIMEDOUT) {
          // timeout occurred 
    } else {
          // operate on x and y 
    }
    pthread_mutex_unlock(&mutex1);
}

/**
 * 
 * Return timestamp (since epoch).
 * 
 * 
 **/
long currentTimeMillis() {
    struct timeb tm;

    ftime(&tm);

    long mill = (tm.time);

    return mill;
}


/**
 * Update the XML documento, applying constraints and checking the server applications.
 * 
 * Specific batch rules can be included here.
 * 
 **/
void * update_thread(void *arg)
{
    // Start time.
    currentTimeMillisStarted = currentTimeMillis();
    printf ("Set start time:%ld\n", currentTimeMillisStarted);
 
    // Run for ever.
    while (1) {


        printf ("Starting checking monitor...\n");

        //if (doc == NULL) {
            doc = updateStatus(doc);
        //}

        // Let's wait for the next time.
        pthread_wait(50);

        // The following code release the document, but if you do so, historic information
        // is completely lost (like uptime for each server). However, the system will use
        // less memory and will be faster.

        //xmlFreeDoc(doc);
        //doc=NULL;
        //xmlCleanupParser();

    }
}

/**
 * Read the configuration from the XML file and generates a CONFIG structure.
 * 
 * 
 * @param   cur     Node Pointer.
 * @return  Configuration
 **/
CONFIG updateConfig(xmlNodePtr cur) {
    CONFIG conf;

    printf ("Loading configuration...\n");

    // Setting default config.
    conf.timeout = 40000;
    conf.alerthits = 10;
    strcpy(conf.emaillist,"");
    strcpy(conf.mailhost,"");
    strcpy(conf.mailport,"25");
    strcpy(conf.proxyhost,"");
    strcpy(conf.proxyport,"80");
    strcpy(conf.proxyPwd,"");
    

    while ( cur != NULL ) {
        
        // For Application, check  HTTP.
        printf ("Name: %s\n", (const char *)cur->name);
        if (!(xmlStrcmp(cur->name,(const xmlChar*)"configuration")) )  {
            xmlNodePtr innercur = cur->children;
            while ( innercur != NULL) {
                printf ("Name..: %s\n", (const char *)innercur->name);

                if (!(xmlStrcmp(innercur->name,(const xmlChar*)"timeout")) )  {
                    if (!xmlIsBlankNode(innercur)) {
                        xmlChar *timeout = xmlNodeGetContent(innercur);

                        
                        conf.timeout = atoi((const char *)timeout);
                        printf ("Read timeout:%d\n", conf.timeout);

                        xmlFree(timeout);
                    }

                }

                if (!(xmlStrcmp(innercur->name,(const xmlChar*)"alerthits")) ) {
                    if (!xmlIsBlankNode(innercur)) {
                        xmlChar *alerthits = xmlNodeGetContent(innercur);

                        
                        conf.alerthits = atoi((const char *)alerthits);
                        printf ("Alert Hits:%d\n", conf.alerthits);

                        xmlFree(alerthits);
                    }
                }

                if (!(xmlStrcmp(innercur->name,(const xmlChar*)"emaillist")) )  {
                    if (!xmlIsBlankNode(innercur)) {
                        xmlChar *emaillist = xmlNodeGetContent(innercur);

                        strcpy(conf.emaillist,(const char *)emaillist);
                        printf ("Read maillist:%s\n", conf.emaillist);

                        xmlFree(emaillist);
                    }

                }

                if (!(xmlStrcmp(innercur->name,(const xmlChar*)"mailserver")) )  {
                    if (!xmlIsBlankNode(innercur)) {
                        xmlChar *mailport = xmlGetProp(innercur, (const xmlChar *)"port");
                        xmlChar *mailhost = xmlGetProp(innercur, (const xmlChar *)"host");

                        strcpy(conf.mailhost,(const char *)mailhost);
                        strcpy(conf.mailport,(const char *)mailport);
                        printf ("Read mailhost:%s\n", conf.mailhost);
                        printf ("Read mailport:%s\n", conf.mailport);

                        xmlFree(mailhost);
                        xmlFree(mailport);
                    }

                }

                if (!(xmlStrcmp(innercur->name,(const xmlChar*)"proxy")) ) {
                    if (!xmlIsBlankNode(innercur)) {
                        xmlChar *proxyhost = xmlGetProp(innercur, (const xmlChar *)"host");
                        xmlChar *proxyport = xmlGetProp(innercur, (const xmlChar *)"port");
                        xmlChar *proxyPwd  = xmlGetProp(innercur, (const xmlChar *)"pwd");

                        strcpy(conf.proxyhost,(const char *)proxyhost);
                        strcpy(conf.proxyport,(const char *)proxyport);
                        strcpy(conf.proxyPwd ,(const char *)proxyPwd);

                        xmlFree(proxyhost);
                        xmlFree(proxyport);
                        xmlFree(proxyPwd);
                    }
                }


                innercur = innercur->next;

            }
        }

        if (!(xmlStrcmp(cur->name,(const xmlChar*)"display")) )  {
            xmlNodePtr innercur = cur->children;
            while ( innercur != NULL) {
                printf ("Name..: %s\n", (const char *)innercur->name);

                if (!(xmlStrcmp(innercur->name,(const xmlChar*)"uptime")) )  {

                    char uptime[20];

                    long timeMillis = currentTimeMillis();

                    timeMillis = timeMillis - currentTimeMillisStarted;
                    
                    sprintf(uptime,"%ld",timeMillis);

                    printf("Uptime:%ld, %ld\n", timeMillis, currentTimeMillisStarted);

                    xmlNodeSetContent(innercur,(const xmlChar*)uptime);

                }

                innercur = innercur->next;

            }
        }


        cur = cur->next;
    }

    return conf;
}

/**
 * Check TCP/IP Status for "connection" nodes.
 *
 **/
void handlerConnectionVerifier(xmlNodePtr cur, xmlChar *name)
{
    if (!(xmlStrcmp(cur->name,(const xmlChar*)"connection")) )  {
    //if ( name != NULL ) {

        xmlNodePtr innercur = cur->children;
        printf ("Check connection %s\r\n",name);
        int error=0;

        xmlChar *ip = xmlGetProp(cur,(const xmlChar *)"ip");
        xmlChar *port = xmlGetProp(cur,(const xmlChar *)"port");
        xmlChar *chkmsg = xmlGetProp(cur,(const xmlChar *)"chkmsg");
        xmlChar *expectedMsg = xmlGetProp(cur, (const xmlChar *)"expmsg");

        int ret = checkConnection((char *)ip,(char *)port, (char *)chkmsg, (char *)expectedMsg);

        xmlFree(ip);xmlFree(port);xmlFree(chkmsg);xmlFree(expectedMsg);

        if (ret == 0) {
            xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Ok");
        } else {
            xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Error");
        }
    }
}



/**
 * For each URL, check weather the amount of time required for checking again has expired
 * or not.
 * 
 * This allow to specify inside the XML file, for each URL, a different value of interval
 * which will allow to check at different speeds for each application.  In general, there are
 * some applications, high speed, that can support high traffic and require very quick responses
 * if something goes wrong.  However, old applications, or not-so-critic application require at
 * least check daily if they are working properly.
 **/
int checkInterval(xmlNodePtr innercur, xmlChar *interval, xmlChar *lastCheck)
{
    // Check time.
    int ret = 1;

    if (interval != NULL) {
        long mill = currentTimeMillis();
        char aux[20];
        sprintf (aux,"%ld",mill);

        if (lastCheck == NULL) {
            ret=1;
            printf ("Last check is null\n");
            xmlSetProp(innercur,(const xmlChar *)"lastcheck",(const xmlChar *)aux);
        } else {
            long lLastCheck = atol((const char *)lastCheck);
            long lInterval = atol((const char *)interval);

            if ( (lLastCheck+lInterval) < mill ) {
                ret=1;
                xmlSetProp(innercur,(const xmlChar *)"lastcheck",(const xmlChar *)aux);
            } else {
                ret=0;
            }

            printf ("Last Check %ld --- %ld  (%ld) (%ld)\n", lLastCheck,lInterval,(lLastCheck+lInterval),mill);

        }
    } else {
        ret=1;
    }
    return ret;
}



/**
 * Update the information in the XML with the status information of all the checked 
 * applications.
 * 
 ***/
xmlDocPtr updateStatus(xmlDocPtr doc)
{
    //xmlDocPtr doc=NULL;
    xmlNodePtr node;

    CONFIG conf;

    if (doc == NULL) {
        doc = xmlParseFile("eclss.xml");
    }

    node = xmlDocGetRootElement(doc);

    // First application element.
    xmlNodePtr cur = node->children;

    // Get actual configuration.
    conf = updateConfig(cur);

    while ( cur != NULL ) {
        xmlChar *name = xmlGetProp(cur,(const xmlChar *)"name");
        
        
        
        if (!(xmlStrcmp(cur->name,(const xmlChar*)"application")) )  {
        //if ( name != NULL ) {

            xmlNodePtr innercur = cur->children;
            printf ("Check for %s\r\n",name);
            char mensaje[256];
            int error=0;
            while ( innercur != NULL ) {
                xmlChar *url = xmlGetProp(innercur, (const xmlChar *)"url");
                xmlChar *proxied = xmlGetProp(innercur, (const xmlChar *)"proxied");
                xmlChar *interval = xmlGetProp(innercur, (const xmlChar *)"interval");
                xmlChar *lastCheck = xmlGetProp(innercur, (const xmlChar *)"lastcheck");
                xmlChar *method = xmlGetProp(innercur, (const xmlChar *)"method");
                xmlChar *postData = xmlGetProp(innercur, (const xmlChar *)"postData");
                xmlChar *errorWaterMark = xmlGetProp(innercur, (const xmlChar *)"errorWaterMark");
                printf ("URL:%s\r\n",url);
                if ( url != NULL && checkInterval(innercur,interval, lastCheck) ) {
                    // Por cada una de las URL a verificar.
                    URL structurl;

                    printf ("Checking URL %s\n",(char *)url);

                    char proxyhost[256];
                    
                    /**
                    int proxyport = 80;
                    char proxyPwd[256] = "user:password";
                    **/

                    unsigned char *encodedProxyPwd;
                    encodedProxyPwd = base64_encode ((unsigned char *)conf.proxyPwd, strlen(conf.proxyPwd)) ;

                    if (proxied != NULL && xmlStrcmp((const xmlChar *)"false",proxied)==0) {
                        strcpy(proxyhost,"");
                    } else {
                        strcpy(proxyhost,conf.proxyhost);
                    }

                    int ret = checkSSLAccess((char *)url,(char *)method,&structurl,conf.timeout,proxyhost,atoi(conf.proxyport),(char *)encodedProxyPwd,(char *)postData,(char *)errorWaterMark);

                    free(encodedProxyPwd);
                    
                    char httpstatus[256];
                    sprintf (httpstatus,"%d",structurl.httpstatus);
                    xmlSetProp(innercur,(const xmlChar *)"httpretcode", (const xmlChar *)httpstatus);
                    char latency[256];
                    sprintf (latency,"%d",structurl.milliseconds);
                    xmlSetProp(innercur,(const xmlChar *)"latency", (const xmlChar *)latency);
                    xmlSetProp(innercur,(const xmlChar *)"urlstatus", (const xmlChar *)structurl.status); 
                    
                    if (ret == 0 ) {
                        xmlSetProp(innercur,(const xmlChar *)"status", (const xmlChar *)"Online");
                    } else {
                        xmlSetProp(innercur,(const xmlChar *)"status", (const xmlChar *)"Error");
                        sprintf(mensaje,"URL: %s</br>Latency: %s</br>Result HTTP: %s</br>Status: %s</br></br>",url,latency,httpstatus,structurl.status);
                        error=-1;
                    }

                    // url, status, statusmsg, httpretcode, latencia,
                    syslog(LOG_INFO,"%s,%s,%s,%s,%s",url,(ret == 0 ? "Online" : "Offline"),structurl.status,httpstatus,latency);


                } else {
                    printf ("Do not check %s\n", (char *)url);
                }
                xmlFree(url);
                if (errorWaterMark != NULL) xmlFree(errorWaterMark);
                if (method != NULL) xmlFree(method);
                if (postData != NULL) xmlFree(postData);
                if (proxied != NULL) xmlFree(proxied);
                if (interval != NULL) xmlFree(interval);
                if (lastCheck != NULL) xmlFree(lastCheck);
                innercur = innercur -> next;
            }

            // Record uptime or downtime.
            xmlChar *oldstatus = xmlGetProp(cur,(const xmlChar *)"status");

            if (error == 0) {
                xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Online");                
                char html[256];
                sprintf(html,"<html><head></head><body>App %s Online</body></html>", name);


                // Uptime Update.
                if (oldstatus==NULL || !(xmlStrcmp(oldstatus,(const xmlChar*)"Error"))) {
                    // Reset Flag.
                    long mill = currentTimeMillis();
                    char aux[20];
                    sprintf (aux,"%ld",mill);
                    xmlSetProp(cur,(const xmlChar *)"timestamp",(const xmlChar *)aux);
                }


               
                //syslog(LOG_INFO,"Application %s Online OK !",name);
            } else {
                xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Error");
                char html[256];
                //sprintf(html,"<html><head></head><body>Aplicacion %s Offline</body></html>", name);
                sprintf(html,"<html><head></head><body>App %s Offline</br>%s</body></html>", name,mensaje);

                // Uptime update
                if ((oldstatus==NULL) || !(xmlStrcmp(oldstatus,(const xmlChar*)"Online"))) {
                    // Reset Flag.
                    long mill = currentTimeMillis();
                    char aux[20];
                    sprintf (aux,"%ld",mill);
                    printf ("Resetting timestamp flag %s\n", aux);
                    xmlSetProp(cur,(const xmlChar *)"timestamp",(const xmlChar *)aux);
                } else {
                    // Check how many times offline reports has been used. Thousands of emails are useless.
                    xmlChar *timestamp = xmlGetProp(cur,(const xmlChar *)"timestamp");
                    xmlChar *ocurrencies = xmlGetProp(cur, (const xmlChar *)"ocurrencies");

                     // Send emails each 4th time.
                    int iOcurrencies = conf.alerthits-4;
                    if (ocurrencies != NULL) {
                        iOcurrencies = atoi((const char *)ocurrencies);
                    } 

                    iOcurrencies++;


                    if ((iOcurrencies) % conf.alerthits == 0) {
                        sendmail(conf.mailhost, conf.mailport, conf.emaillist,html);
                    }


                    //if ( ( ( currentTimeMillis() - atol((const char *)timestamp) ) > 200 ) && 
                    //     ( ( currentTimeMillis() - atol((const char *)timestamp) ) < 250 ) ) {
                    //    sendmail(conf.mailhost, conf.mailport, conf.emaillist,html);
                    //}
                    
                    char strOcurrencies[10];
                    sprintf (strOcurrencies,"%d",iOcurrencies);

                    printf ("Ocurrencias de Offline %s\n", strOcurrencies);

                    xmlSetProp(cur,(const xmlChar *)"ocurrencies", (const xmlChar *)strOcurrencies);

                    xmlFree(ocurrencies);
                    xmlFree(timestamp);

                }

                //syslog(LOG_INFO,"Application %s Offline !",name);
            }

            long mill = currentTimeMillis();
            xmlChar *timestamp = xmlGetProp(cur, (const xmlChar *)"timestamp");
            long ltimestamp = atol((const char *)timestamp);

            char aux[20];
            sprintf (aux,"%ld",(mill-ltimestamp));

            xmlSetProp(cur,(const xmlChar *)"timelife",(const xmlChar *)aux);


            xmlFree(timestamp);
            xmlFree(oldstatus);
        }

        // Check other type of NO-HTTP connections.
        handlerConnectionVerifier(cur,name);

        xmlFree(name);
        

        cur = cur->next;
    }

    printf ("Finish updating xml..\n");

    // Release node.
    //xmlFreeNode(node);

    // Release parser memory
    //xmlCleanupParser();

    // Release the document
    //xmlFreeDoc(doc);
    //doc=NULL;

    return doc;

}

void test() 
{
    /**
            // Send reply
            // HTTP Headers.   EXPERIMENTAL
            strcpy(buf,"HTTP/1.0 200 Ok\r\n");
            strcat(buf,"Content-type: multipart/x-mixed-replace;boundary=---ThisRandomString---\r\n");
            strcat(buf,"Date: Mon, 21 Mar 2005 12:10:37 GMT\r\n");
            strcat(buf,"Server: MyApache/1.3.28 (Unix) mod_jk/1.2.5 mod_ssl/2.8.15 OpenSSL/0.9.7b\r\n");
            strcat(buf,"\r\n");
            strcat(buf,"---ThisRandomString---\r\n");
            val = write ( sd, buf, strlen(buf));


            while ( val != -1 ) {
                strcpy(buf,"Content-Type: text/html; charset=iso-8859-1\r\n");

                strcat(buf,"\r\n");

                val = write ( sd, buf, strlen(buf));

                if (val == -1)
                    break;



                if (doc == NULL) {
                    doc = updateStatus(doc);
                }

                if (xsltProcess(sd,doc,"eclss.xsl") == -1) {
                    printf ("Error al procesar el XSL.!");
                    break;
                }

                strcpy(buf,"---ThisRandomString---\r\n");
                val = write ( sd, buf, strlen(buf));

                pthread_wait(5);
            }

    **/
}





/**
 * Process each request.
 *
 * @param   Socket Descriptor.
 *
 **/
void * request_handler(void *arg)
{
    int sd;
    int val = 0;
    char inbuf[4096];
    char buf[4096];
    size_t len;

    sd = *((int*)arg);

    while ( (val = read( sd, inbuf, 4098))>0 ) {
        if (val > 0 ) {
            inbuf[val]='\0';
            //printf ("%s", inbuf);

            // TODO: This parser sucks, sorry.
            if ( strstr(inbuf,"GET /help")!=NULL ) {
                strcpy(buf,"HTTP/1.1 200 Ok\r\n");
                strcat(buf,"Date: Mon, 21 Mar 2005 12:10:37 GMT\r\n");
                strcat(buf,"Server: Apache/1.3.28 (Unix) mod_jk/1.2.5 mod_ssl/2.8.15 OpenSSL/0.9.7b\r\n");
                strcat(buf,"Connection: close\r\n");
                strcat(buf,"Content-Type: text/html; charset=iso-8859-1\r\n");

                strcat(buf,"\r\n");

                val = write ( sd, buf, strlen(buf));

                xmlDocPtr doc;
                xmlNodePtr node;

                doc = xmlNewDoc((const xmlChar *)"1.0");

                node = xmlNewNode((xmlNs *)NULL,(const xmlChar *)"document");

                xmlDocSetRootElement(doc,node);

                printf ("Ready to XSL Processing....\n");
    
                if (xsltProcess(sd,doc,"eclsshelp.xsl") == -1) {
                    printf ("ERROR At XSL.!");
                    break;
                }

                xmlFree(doc);

                //raise(SIGINT);

                break;

            }
            if ( strstr(inbuf,"GET /stop")!=NULL ) {
                strcpy(buf,"HTTP/1.1 200 Ok\r\n");
                strcat(buf,"Date: Mon, 21 Mar 2005 12:10:37 GMT\r\n");
                strcat(buf,"Server: Apache/1.3.28 (Unix) mod_jk/1.2.5 mod_ssl/2.8.15 OpenSSL/0.9.7b\r\n");
                strcat(buf,"Connection: close\r\n");
                strcat(buf,"Content-Type: text/html; charset=iso-8859-1\r\n");

                strcat(buf,"\r\n");

                strcat(buf,"<html><head></head><body>The ECLSS Application has been stopped.</body></html>");

                val = write ( sd, buf, strlen(buf));

                //raise(SIGINT);

                shouldistop = 1;

                break;

            }
            if ( strstr(inbuf,"GET /reload")!=NULL ) {
                // Load from the XML file and reset everything else.
                strcpy(buf,"HTTP/1.1 200 Ok\r\n");
                strcat(buf,"Date: Mon, 21 Mar 2005 12:10:37 GMT\r\n");
                strcat(buf,"Server: Apache/1.3.28 (Unix) mod_jk/1.2.5 mod_ssl/2.8.15 OpenSSL/0.9.7b\r\n");
                strcat(buf,"Connection: close\r\n");
                strcat(buf,"Content-Type: text/html; charset=iso-8859-1\r\n");

                strcat(buf,"\r\n");

                strcat(buf,"<html><head></head><body>The configuration has been reloaded from the config file.  The register values has been resetted.</body></html>");

                val = write ( sd, buf, strlen(buf));

                // Se libera el documento, en la proxima ejecucion el mismo se actualizara desde el archivo.
                // Release the document, so the next time it will read it from the file.
                xmlFreeDoc(doc);
                doc=NULL;
                break;

                
            }
            if ( strstr(inbuf,"GET / ")==NULL ) {
                // Error PAGE
                strcpy(buf,"HTTP/1.1 400 Not Found\r\n");
                strcat(buf,"Date: Mon, 21 Mar 2005 12:10:37 GMT\r\n");
                strcat(buf,"Server: Apache/1.3.28 (Unix) mod_jk/1.2.5 mod_ssl/2.8.15 OpenSSL/0.9.7b\r\n");
                strcat(buf,"Connection: close\r\n");
                strcat(buf,"Content-Type: text/html; charset=iso-8859-1\r\n");

                strcat(buf,"\r\n");

                strcat(buf,"<html><head></head><body>The page you are requesting was not found.</body></html>");

                val = write ( sd, buf, strlen(buf));
                break;
            }
            

            // This is a good one, show what you have.
            
            // HTTP Headers.
            strcpy(buf,"HTTP/1.1 200 Ok\r\n");
            strcat(buf,"Date: Mon, 21 Mar 2005 12:10:37 GMT\r\n");
            strcat(buf,"Server: MyApache/1.3.28 (Unix) mod_jk/1.2.5 mod_ssl/2.8.15 OpenSSL/0.9.7b\r\n");
            strcat(buf,"Connection: close\r\n");
            strcat(buf,"Content-Type: text/html; charset=iso-8859-1\r\n");

            strcat(buf,"\r\n");

            val = write ( sd, buf, strlen(buf));

            if (val<0) {
                printf ("Broken pipe...\n");
                break;
            }

            // The status can be updated on-demand.

            if (doc == NULL) {
                doc = updateStatus(doc);
            }

            printf ("Ready to XSL Processing....\n");

            if (xsltProcess(sd,doc,"eclss.xsl") == -1) {
                printf ("Error AT XSL.!");
                break;
            }

          
            //xmlFreeDoc(doc);
            //doc=NULL;
            //xmlCleanupParser();

            //sprintf (buf,"<html><head></head><body><script>window.location.reload()</script></body></html>");
            //val = write( sd,buf, strlen(buf));


            break;

        } else {
            printf ("Error from the client (Broken pipe)\n");
        }
    }

    close(sd);
    printf ("Finished processing request...\n");
    int d=0;
    pthread_exit(&d);
}

