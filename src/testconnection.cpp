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

// Flag to detect if the main process must stop execution or not.
int shouldistop;
int msd;

int DEBUG=1;

int main(int argc, char **argv)
{
    // Por cada una de las URL a verificar.
    URL structurl;
    CONFIG conf;

    char url[256];
    char proxyhost[256];
    char method[256];
    char encodedProxyPwd[256];
    char postData[256];
    char errorWaterMark[256];
    char expectedWaterMark[256];


    strcpy(url, "https://www.buenosaires.gob.ar/coronavirus/vacunacion-covid-19/personal-de-salud");

    printf ("Checking URL %s\n",(char *)url);

    strcpy(proxyhost,"");

    conf.timeout = 20000;

    strcpy(method,"GET");
    strcpy(encodedProxyPwd,"");
    strcpy(postData,"");
    strcpy(errorWaterMark,"");
    strcpy(expectedWaterMark,"Habilitaremos nuevamente los turnos para la vacunación una vez que se confirme la llegada de una próxima entrega de vacunas a la Ciudad.");
    strcpy(expectedWaterMark,"Habilitaremos nuevamente los turnos");

    int ret = checkSSLAccess((char *)url,(char *)method,&structurl,conf.timeout,proxyhost,atoi(conf.proxyport),(char *)encodedProxyPwd,(char *)postData,(char *)errorWaterMark, (char *)expectedWaterMark);


    strcpy(url, "http://www.google.com");

    printf ("Checking URL %s\n",(char *)url);

    strcpy(proxyhost,"");

    conf.timeout = 2000;

    strcpy(method,"GET");
    strcpy(encodedProxyPwd,"");
    strcpy(postData,"");
    strcpy(errorWaterMark,"");
    strcpy(expectedWaterMark,"");

    ret = checkSSLAccess((char *)url,(char *)method,&structurl,conf.timeout,proxyhost,atoi(conf.proxyport),(char *)encodedProxyPwd,(char *)postData,(char *)errorWaterMark, (char *)expectedWaterMark);


    return ret;
}
