/******************************************************************************
 * https.cpp
 *
 * TESting XML.
 *
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
#include "handler.h"
#include "teradata.h"

static xmlDocPtr doc=NULL;

static long currentTimeMillisStarted=0;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

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


long currentTimeMillis() {
    struct timeb tm;

    ftime(&tm);

    long mill = (tm.time);

    return mill;
}



void * update_thread(void *arg)
{
    currentTimeMillisStarted = currentTimeMillis();
    printf ("Set start time:%ld\n", currentTimeMillisStarted);
 
    while (1) {


        printf ("Starting monitor ...\n");

        //if (doc == NULL) {
            doc = updateStatus(doc);
        //}

        int sd = creat("f.txt",O_WRONLY);
        if (xsltProcess(sd,doc,"eclss.xsl") == -1) {
            printf ("Error XSL.!");
            break;
        }
        close(sd);

        //pthread_wait(40);

        //xmlFreeDoc(doc);
        //doc=NULL;
        //xmlCleanupParser();

    }
}


CONFIG updateConfig(xmlNodePtr cur) {
    CONFIG conf;

    printf ("Loading configuration...\n");

    conf.timeout = 40000;
    strcpy(conf.emaillist,"sss@fff.ddd.ss");
    strcpy(conf.mailhost,"ddd");
    strcpy(conf.mailport,"110");
    

    while ( cur != NULL ) {
        
        // Para los nodos application verifica hasta HTTP.
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


    conf = updateConfig(cur);

    while ( cur != NULL ) {
        xmlChar *name = xmlGetProp(cur,(const xmlChar *)"name");
        
        if (!(xmlStrcmp(cur->name,(const xmlChar*)"application")) )  {
        //if ( name != NULL ) {

            xmlNodePtr innercur = cur->children;
            printf ("Check for %s\r\n",name);
            int error=0;
            while ( innercur != NULL ) {
                xmlChar *url = xmlGetProp(innercur, (const xmlChar *)"url");
                printf ("URL:%s\r\n",url);
                if ( url != NULL ) {
                    URL structurl;
                    if (0 == 0 ) {
                        xmlSetProp(innercur,(const xmlChar *)"status", (const xmlChar *)"Online");
                    } else {
                        xmlSetProp(innercur,(const xmlChar *)"status", (const xmlChar *)"Error");
                        error=-1;
                    }
                    char httpstatus[256];
                    sprintf (httpstatus,"%d",structurl.httpstatus);
                    xmlSetProp(innercur,(const xmlChar *)"httpretcode", (const xmlChar *)httpstatus);
                    char latency[256];
                    sprintf (latency,"%d",structurl.milliseconds);
                    xmlSetProp(innercur,(const xmlChar *)"latency", (const xmlChar *)latency);
                }
                xmlFree(url);
                innercur = innercur -> next;
            }

            xmlChar *oldstatus = xmlGetProp(cur,(const xmlChar *)"status");

            if (error == 0) {
                xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Online");                
                char html[256];
                sprintf(html,"<html><head></head><body>Aplicacion %s Online</body></html>", name);


                if (oldstatus==NULL || !(xmlStrcmp(oldstatus,(const xmlChar*)"Error"))) {
                    // Reset Flag.
                    long mill = currentTimeMillis();
                    char aux[20];
                    sprintf (aux,"%ld",mill);
                    xmlSetProp(cur,(const xmlChar *)"timestamp",(const xmlChar *)aux);
                }


               
                syslog(LOG_INFO,"Application %s Online OK !",name);
            } else {
                xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Error");
                char html[256];
                sprintf(html,"<html><head></head><body>Aplicacion %s Offline</body></html>", name);

                if ((oldstatus==NULL) || !(xmlStrcmp(oldstatus,(const xmlChar*)"Online"))) {
                    // Reset Flag.
                    long mill = currentTimeMillis();
                    char aux[20];
                    sprintf (aux,"%ld",mill);
                    printf ("Resetting timestamp flag %s\n", aux);
                    xmlSetProp(cur,(const xmlChar *)"timestamp",(const xmlChar *)aux);
                    //sendmail("coscarelli@visa.com.ar;cored@visa.com.ar;ramele@visa.com.ar;ruizm@visa.com.ar;cachenot@visa.com.ar;IMCEAEX-_O=VISA+20ARGENTINA_OU=VISA2_CN=GCIA+5FPRODUCCION_CN=DESARR+5FSISTEMAS_CN=PERS+5FEXT+5FDESARROLLO_CN=CASERO@visa.com.ar",html);
                    

                }
                syslog(LOG_INFO,"Application %s Offline !",name);
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

        if (!(xmlStrcmp(cur->name,(const xmlChar*)"connection")) )  {
        //if ( name != NULL ) {

            xmlNodePtr innercur = cur->children;
            printf ("Check connection %s\r\n",name);
            int error=0;

            xmlChar *ip = xmlGetProp(cur,(const xmlChar *)"ip");
            xmlChar *port = xmlGetProp(cur,(const xmlChar *)"port");
            xmlChar *chkmsg = xmlGetProp(cur,(const xmlChar *)"chkmsg");

            int ret = 0;

            xmlFree(ip);xmlFree(port);xmlFree(chkmsg);

            if (ret == 0) {
                xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Ok");
            } else {
                xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Error");
            }
        }


        if (!(xmlStrcmp(cur->name,(const xmlChar*)"teradata")) )  {
        //if ( name != NULL ) {

            printf ("Check connection %s\r\n",name);
            int error=0;

            xmlChar *logonstring = xmlGetProp(cur,(const xmlChar *)"logonstring");

            int ret = 0;
            char connectioncount[]="10";

            xmlFree(logonstring);

            if (ret == 0) {
                xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Ok");
                xmlSetProp(cur,(const xmlChar *)"message",(const xmlChar *)connectioncount);
            } else {
                xmlSetProp(cur,(const xmlChar *)"status",(const xmlChar *)"Error");
            }
        }

        xmlFree(name);

        cur = cur->next;
    }


    //xmlFreeNode(node);


    //xmlCleanupParser();

    //xmlFreeDoc(doc);
    //doc=NULL;

    return doc;

}

int main(int argc,char *argv[])
{
    update_thread(NULL);
}

