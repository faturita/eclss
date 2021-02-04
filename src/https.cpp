/******************************************************************************
 * https.cpp
 *
 * HTTPS handler.
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
#include <string>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timeb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>



#include <sys/ioctl.h>
#include <fcntl.h>

#include <errno.h>


// OpenSSL
#include <stdio.h>
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/pkcs12.h>
#include <openssl/ssl.h>

#include "https.h"

extern int DEBUG;

unsigned int atoh(char *ap)
{
    char *p;
    unsigned int n;
    int digit,lcase;

    p = ap;
    n = 0;
    while(*p == ' ' || *p == '	')
        p++;

    if(*p == '0' && ((*(p+1) == 'x') || (*(p+1) == 'X')))
        p+=2;

    while ((digit = (*p >= '0' && *p <= '9')) ||
        (lcase = (*p >= 'a' && *p <= 'f')) ||
        (*p >= 'A' && *p <= 'F')) {
        n *= 16;
        if (digit)	n += *p++ - '0';
        else if (lcase)	n += 10 + (*p++ - 'a');
        else		n += 10 + (*p++ - 'A');
    }
    return(n);
}

void bookeep(char *path, char * resultcode, long elapsedtime, int iapperror)
{
    char timestamp[256];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(timestamp, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    FILE *fp = fopen("bot.csv","a");
    if (fp != NULL)
    {
        char message[1024];
        sprintf(message,"%s, %s,%s,%ld,%d\n", timestamp, path, resultcode,elapsedtime,iapperror );
        fputs(message, fp);
        fclose(fp);
    }
}

/**
 * Java-Like Get index of.
 **/ 
int getIndexOf(char *urlstring,char d, int times) 
{
    for(int i=0;i<strlen(urlstring);i++) {
        if (urlstring[i] == d) {
            times--;
        }
        if (times == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * URL Parser
 **/ 
URL parseURL(char *urlstring)
{
    URL url;

    memset(&url, 0, sizeof(url));

    int fslash;
    
    if (urlstring[4] == 's') {
        fslash = 5;  // if https
    } else {
        fslash = 4;
    }

    strncpy(url.proto,urlstring,fslash);
    url.proto[fslash]='\0';
    int slash = getIndexOf(urlstring,'/',3);
    if (slash == -1)
    {
        slash = strlen(urlstring);
        strncpy(url.host,urlstring+3+fslash,slash-(3+fslash));
        url.host[slash-(3+fslash)]='\0';
        strncpy(url.path,urlstring+slash, strlen(urlstring)-slash);
        strcpy(url.path, "/");
    } else {
        strncpy(url.host,urlstring+3+fslash,slash-(3+fslash));
        url.host[slash-(3+fslash)]='\0';
        strncpy(url.path,urlstring+slash, strlen(urlstring)-slash);
        url.path[strlen(urlstring)-slash]='\0';
    }


    if (getIndexOf(url.host,':',1)!=-1) {
        char port[10];
        strcpy(port,url.host+getIndexOf(url.host,':',1)+1);
        url.port = atoi(port);
        url.host[getIndexOf(url.host,':',1)]='\0';
    } else {
        if ( strcmp(url.proto,"https")==0 ) {
            url.port = 443;
        } else {
            url.port = 80;
        }
    }

    return url;
}

/**
 * With PROXY there is a neccesary pre connection to the proxy, specifying which whom I would
 * like to connect.
 * 
 * Use the "sd" connector and order the proxy to establish a new connection with "host":"port".
 * The password (in BASE64) is "encodedpwd".
 * I do not know why the proxy password is so weak (it is not encrypted, only encoded).
 * 
 ***/ 
void pre_https_proxy_dialog(int sd,char host[256],int port, char encodedpwd[256])
{
    char buf[4096];
    char inbuf[4096];
    char aux[4096];
    int val;
    char resultcode[256];

    sprintf(buf,"CONNECT %s:%d HTTP/1.1\r\n",host, port);
    sprintf(aux,"Host: %s\r\n",host);
    strcat(buf,aux);
    sprintf(aux,"Proxy-Authorization: Basic %s\r\n", encodedpwd);
    strcat(buf,aux);
    
    strcat(buf,"\r\n");

    printf ("Pre headers %s\n", buf);

    val = write ( sd, buf, strlen(buf));

    while ( (val = read( sd, inbuf, 4096))!=0 ) {
	    if (val > 0 ) {
	        inbuf[val] = '\0';

            printf ("%s",inbuf);
		
            // Sets ResultCode
            if (strstr(inbuf,"HTTP/1.")!=NULL) {
                strncpy(resultcode,strstr(inbuf,"HTTP/1.")+9,3);
                break;
            } 	    
        }
    }

}

/**
 * This is the most important function of the entire program.
 * 
 * Establish an initial http dialog with session-established.
 * 
 * Depending on the field URL checks the security parameters of the connection
 * and apply the different aproaches to each one of them.
 * 
 * Proxy HTTP and HTTPS:
 * -The connection should be targeted to proxy host:proxy port.
 * -Authentication handling:
 * Attach: Proxy-Authentication: Basic dmV4cmFyb2Q6a3VydW1hZ2FzdWtp
 * -Host Field: Target Host.
 * 
 * Proxy HTTP:
 * -PATH field: http://url/
 * 
 * Proxy HTTPS:
 * -METHOD field: CONNECT
 * -PATH field: host:443 (or whatever port)
 * -Proxy-Authorization: Basic dmV4cmFyb2Q6ZGVuc2hhZ2FzdWtp
 * (The hash is "username:password" in BASE64)
 * 
 * When 200 Connection Established is received, start SSL dialog and
 * HTTP proxy dialog.
 *
 **/
int http_dialog(int sd,SSL *ssl, URL *url,char *postData, char *jsession, int iTimeout, char *encodedPwdProxy, char *errorWaterMark, char *expectedWaterMark) {
    
    char buf[4096];
    char aux[4096];
    char inbuf[4096];
    char *hinbuf;
    int randd;
    char resultcode[4];
    time_t elapsedTime;
    int i;
    int iapperror=0;
    char headers[4096];
                
    ssize_t val;

    randd = 0+(int)((10-1)*1.0*rand()/(RAND_MAX+1.0));

    randd = (randd * 2) + 1;

    // Send HTTP Headers.
    sprintf(buf,"%s %s HTTP/1.1\r\n",url->method,url->path);
    strcat(buf,"Accept: application/vnd.ms-excel, application/msword, application/vnd.ms-powerpoint, image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-comet, application/x-shockwave-flash, */*\r\n");
    strcat(buf,"Accept-Language: es-ar\r\n");
    //strcat(buf,"Accept-Encoding: gzip, deflate\r\n");
    strcat(buf,"Accept-Encoding: utf-8\r\n");
    strcat(buf,"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 4.0)\r\n");

    // Host, required in HTTP 1.1
    sprintf(aux, "Host: %s:%d\r\n", url->host, url->port);
    strcat(buf,aux);

    // Connection keep-alive, without cache.
    strcat(buf,"Connection: Keep-Alive\r\n");
    strcat(buf,"Cache-Control: no-cache\r\n");

    if ( url->proxied ) {
        sprintf(aux,"Proxy-Authorization: Basic %s\r\n",encodedPwdProxy);
        strcat(buf,aux);
    }

    // If present, send cookie.
    if ( jsession != NULL && (strlen(jsession)>0) ) {
        sprintf(aux,"Cookie: %s\r\n",jsession);
        strcat(buf,aux);
    }
    
    // Length of the request (only if url->method=POST)
    if ( strcmp(url->method,"POST")==0 ) {
        strcat(buf,"Content-Type: application/x-www-form-urlencoded\r\n");
        sprintf(aux, "Content-length: %d \r\n", strlen(postData));
        strcat(buf, aux);
        // Final CR-LF
        strcat(buf, "\r\n");
    
        // Add POST DATA.
        strcat(buf,postData);
    } else {
        // Final CR-LF
        strcat(buf, "\r\n");
    }

    
    // Ok, send info (NOTE: different if using HTTP or HTTPS)
    if (strcmp(url->proto,"https")==0) {
        val = SSL_write (ssl,buf, strlen(buf));
    } else {   
        // HTTP
        val = write ( sd, buf, strlen(buf));
    }

    if (val < 0) {
        // The pipe is broken, error.
        printf("Broken pipe\n");
        return -1;
    }
    
    // Sets the result code to ZERO
    strcpy(resultcode, "000");

    // Start Timer.
    struct timeb tm,tm2;

    elapsedTime = time(NULL);
    ftime(&tm);

    if (DEBUG)
        printf ("%s\n",buf);

    printf ("Waiting for response....\n");

    //while ( (val = read( sd, inbuf, 4096))!=0 ) 
    // Remember that the order inside the conditions is very important.

    int h=0;

    char response[128000];
    int respcounter=0;

    char conbuf[1];


    // Headers
    char HTTPEncoding[4096];
    char HTTPTransferEncoding[4096];
    char HTTPContentLength[4096]; unsigned int contentlength=0;

    memset(HTTPEncoding,0,sizeof(HTTPEncoding));
    memset(response,0,sizeof(response));


    // Headers
    while ( ( (ssl!=NULL) && (val = SSL_read(ssl,conbuf,1))!=0 ) ||
            ( (val = read( sd, conbuf, 1))!=0 )  )
    {
        char header[4096];

        if (val > 0 ) {
            header[h++] = conbuf[0];

            response[respcounter++]=conbuf[0];

            if (strstr(response,"\r\n\r\n") != NULL) {
                printf("End of headers %s\n",response);
                break;
            }

            if ( strstr(header, "\r\n") != NULL) {
                hinbuf = strstr(header, "\r\n");
                hinbuf[0] = '\0';

                memset(inbuf,0,sizeof(4096));
                strcpy(inbuf, header);

                printf("Header: %s\n", header);
                h=0;

                // Get the cookie if present.
                if ( strstr(inbuf, "Set-Cookie") != NULL ) {

                    hinbuf = strstr(inbuf,"Set-Cookie")+12;

                    for(i=0;hinbuf< strstr( strstr(inbuf,"Set-Cookie")+12,";");hinbuf+=sizeof(char)) {
                        aux[i]=*(hinbuf);
                        i++;
                    }

                    aux[i]='\0';
                    printf ("Read Cookie: %s\r\n", aux);
                    strcpy(jsession, aux);
                }

                // Sets ResultCode
                if (strstr(inbuf,"HTTP/1.")!=NULL) {
                    strncpy(resultcode,strstr(inbuf,"HTTP/1.")+9,3);
                    printf("Result Code: %s\n", resultcode);
                }

                if (strstr(inbuf,"Content-Encoding: ")!=NULL)
                {
                    strncpy(HTTPEncoding, strstr(inbuf,"Content-Encoding: ")+strlen("Content-Encoding: "),strlen(inbuf));
                    printf("Encoding: %s\n", HTTPEncoding);
                }

                if (strstr(inbuf,"Transfer-Encoding: ")!=NULL)
                {
                    strncpy(HTTPTransferEncoding, strstr(inbuf,"Transfer-Encoding: ")+strlen("Transfer-Encoding: "),strlen(inbuf));
                    printf("Transfer encoding: %s\n", HTTPTransferEncoding);
                }

                if (strstr(inbuf,"Content-Length: ")!= NULL)
                {
                    strncpy(HTTPContentLength, strstr(inbuf,"Content-Length: ")+strlen("Content-Length: "),strlen(inbuf));
                    printf("Content Length: %s\n", HTTPContentLength);
                    contentlength = atoi(HTTPContentLength);
                }

            }
        }

        // Get timestamp

        ftime(&tm2);

        int mill = (tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm);

        // Configurable will be better.
        // que (strcmp(resultcode,"000")!=0)
        if ( mill > iTimeout ) {
            printf("Timeout\n");
            break;
        }
    }

    if ( strstr(HTTPTransferEncoding,"chunked") != NULL)
    {
        unsigned int chunklength=0;
        h=0;
        // Chunked content length
        while ( ( (ssl!=NULL) && (val = SSL_read(ssl,conbuf,1))!=0 ) ||
                ( (val = read( sd, conbuf, 1))!=0 )  )
        {
            char length[4096];

            if (val > 0 ) {
                length[h++] = conbuf[0];

                response[respcounter++]=conbuf[0];

                if (strstr(length,"\r\n") != NULL) {
                    hinbuf = strstr(length, "\r\n");
                    hinbuf[0] = '\0';

                    memset(inbuf,0,sizeof(4096));
                    strcpy(inbuf, length);

                    chunklength = atoh(length);

                    printf("Length: %s\n", length);
                    printf("Length: %d\n", chunklength);
                    h=0;
                    break;

                }
            }

            // Get timestamp

            ftime(&tm2);

            int mill = (tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm);

            // Configurable will be better.
            // que (strcmp(resultcode,"000")!=0)
            if ( mill > iTimeout ) {
                printf("Timeout\n");
                break;
            }
        }

        h=0;
        char buffer[chunklength+1];
        memset(buffer,0,strlen(buffer));
        // Chunked content length
        while ( ( (ssl!=NULL) && (val = SSL_read(ssl,conbuf,1))!=0 ) ||
                ( (val = read( sd, conbuf, 1))!=0 )  )
        {

            if (val > 0 ) {
                buffer[h++] = conbuf[0];

                response[respcounter++]=conbuf[0];

                if (h==chunklength)
                {
                    buffer[chunklength] = '\0';
                    if (strstr(HTTPEncoding,"gzip") != NULL)
                    {

                        FILE *pf = fopen("f.txt.gz","w");

                        fwrite(buffer,1,chunklength,pf);

                        fclose(pf);


                        system("gunzip -f f.txt.gz");


                        pf = fopen("f.txt", "r");

                        if(pf == NULL) {
                            printf("Cannot open file\n");
                            exit(1);
                        }


                        iapperror = 0;
                        while ( fgets(inbuf, 4096, pf) != NULL)
                        {
                            if ( expectedWaterMark != NULL)
                            {
                                if ( strstr(inbuf,expectedWaterMark)!=NULL )
                                {
                                    printf("Found !\n");
                                } else {
                                    // When the watermark is not found I want to flag it as an error.
                                    iapperror = 1;
                                }
                            }

                            if (strstr(inbuf,"</HTML>") != NULL) {
                                // Heuristic to speed up the processing time.
                                // Content-Length could also work.
                                printf("all processed\n");
                            }
                        }
                    } else {
                        iapperror=0;
                        if ( expectedWaterMark != NULL)
                        {
                            if ( strstr(buffer,expectedWaterMark)!=NULL )
                            {
                                printf("Found !\n");
                            } else {
                                // When the watermark is not found I want to flag it as an error.
                                iapperror = 1;
                            }
                        }

                        if (strstr(buffer,"</HTML>") != NULL) {
                            // Heuristic to speed up the processing time.
                            // Content-Length could also work.
                            printf("all processed\n");
                        }
                    }

                    if (iapperror==1)
                    {
                        system("osascript -e 'tell application \"Messages\" to send \"Página del Gobierno de la Ciudad Vacunas\"  to buddy \"Julieta Besteiro\"'");
                    }


                    break;
                }


            }

            // Get timestamp

            ftime(&tm2);

            int mill = (tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm);

            // Configurable will be better.
            // que (strcmp(resultcode,"000")!=0)
            if ( mill > iTimeout ) {
                printf("Timeout\n");
                break;
            }
        }

        printf("Bytes read %d\n",h);

        chunklength=0;
        h=0;
        // Chunked content length
        while ( ( (ssl!=NULL) && (val = SSL_read(ssl,conbuf,1))!=0 ) ||
                ( (val = read( sd, conbuf, 1))!=0 )  )
        {
            char length[4096];

            if (val > 0 ) {
                length[h++] = conbuf[0];

                response[respcounter++]=conbuf[0];

                if (strstr(length,"\r\n") != NULL) {
                    hinbuf = strstr(length, "\r\n");
                    hinbuf[0] = '\0';

                    memset(inbuf,0,sizeof(4096));
                    strcpy(inbuf, length);

                    chunklength = atoh(length);

                    printf("Length: %s\n", length);
                    printf("Length: %d\n", chunklength);
                    h=0;
                    break;

                }
            }

            // Get timestamp

            ftime(&tm2);

            int mill = (tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm);

            // Configurable will be better.
            // que (strcmp(resultcode,"000")!=0)
            if ( mill > iTimeout ) {
                printf("Timeout\n");
                break;
            }
        }

    } else {

        printf("Waiting body...\n");
        h=0;
        // HTTP Content Length
        while ( ( (ssl!=NULL) && (val = SSL_read(ssl,conbuf,1))!=0 ) ||
                ( (val = read( sd, conbuf, 1))!=0 )  )
        {
            char length[4096];

            if (val > 0 ) {
                length[h++] = conbuf[0];

                response[respcounter++]=conbuf[0];

                if (h==contentlength)
                {
                    // Check for the watermark inside the reply (IF WATERMARK IN, ERROR)
                    if ( errorWaterMark != NULL && strstr(inbuf,errorWaterMark)!=NULL ) {
                        iapperror = 1;
                        break;
                    }
                }
            }

            // Get timestamp

            ftime(&tm2);

            int mill = (tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm);

            // Configurable will be better.
            // que (strcmp(resultcode,"000")!=0)
            if ( mill > iTimeout ) {
                printf("Timeout\n");
                break;
            }
        }
    }

    /**

    while ( ( (ssl!=NULL) && (val = SSL_read(ssl,inbuf,4096))!=0 ) ||
            ( (val = read( sd, inbuf, 4096))!=0 )  )
    {
    
	    if (val > 0 ) {
	        inbuf[val] = '\0';
            if (DEBUG) 
	            printf ("%s", inbuf);
			
            // Get the cookie if present.
	        if ( strstr(inbuf, "Set-Cookie") != NULL ) {
		    
    		    hinbuf = strstr(inbuf,"Set-Cookie")+12;
		    
                for(i=0;hinbuf< strstr( strstr(inbuf,"Set-Cookie")+12,";");hinbuf+=sizeof(char)) {
	                aux[i]=*(hinbuf);
	                i++;
                } 
		    
		        aux[i]='\0';
		        printf ("Read Cookie: %s\r\n", aux);
		        strcpy(jsession, aux);
    	    }
	    
            // Sets ResultCode
            if (strstr(inbuf,"HTTP/1.")!=NULL) {
                strncpy(resultcode,strstr(inbuf,"HTTP/1.")+9,3);
                
                // If I need to check HTTP being here is enough because I already have
                // a working connection (with or without SSL) and a valid HTTP response.
                
                // If I want to check for more information I need to check what the server
                // is saying.
                if (errorWaterMark == NULL) {
                    // NoWatermark, for this application only HTTP is required.
                    break;
                }
            }

            
            // Check for the watermark inside the reply (IF WATERMARK IN, ERROR)
            if ( errorWaterMark != NULL && strstr(inbuf,errorWaterMark)!=NULL ) { 
                iapperror = 1;
                break;
            }

            if (strstr(inbuf,"</HTML>") != NULL) {
                // Heuristic to speed up the processing time.
                // Content-Length could also work.
                break;
            }

        }

        // Get timestamp

        ftime(&tm2);

        int mill = (tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm);

        // Configurable will be better.
        // que (strcmp(resultcode,"000")!=0)
        if ( mill > iTimeout ) {
            break;
        }
	
    }**/
    

    elapsedTime = time(NULL) - elapsedTime;
    ftime(&tm2);

    printf ("Session %s - Page %s - ResultCode %s - Elapsed Time %d\n", jsession, url->path, resultcode, (tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm)); 

    bookeep(url->host,resultcode,(tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm),iapperror );

    url->milliseconds = (tm2.time*1000+tm2.millitm)-(tm.time*1000+tm.millitm);
    url->httpstatus = atoi(resultcode);
    
    // Se considera ok, acceso denegado, redirect, moved permanently como respuestas validas.
    // Responses OK,
    //      200 OK
    //      401 Access Denied
    //      302 Redirect
    //      301 Moved Permanently.
    //
    // FIXME: Configure this.
    
    if (strcmp(resultcode, "200")==0 || strcmp(resultcode,"401")==0 || strcmp(resultcode,"302")==0 || strcmp(resultcode,"301")==0 ) {
        // OK the HTTP response and also the code seems to be OK

        // Check for the WATERMARK flag.
        if (iapperror != 0) {
            sprintf(url->status,"The application is generating messages with error.");
            return -1;
        } else {
            return 0;
        }

    	return 0;
    } else if (strcmp(resultcode, "000")==0) {
        sprintf(url->status,"Timeout waiting for the server reply. This message is WARNING, because the application could be only SLOW.  Please, be aware of the situation.");
        return -1;
    } else {
        // TODO: Put a table with the available response codes.
        sprintf(url->status,"The application is REPLYing with ERROR.  Please check it up.");
        return -1;
    }    
}

/**
 * Use the "sd" to connect to "url" using the ssl structures.
 * 
 * Open a SSL connection to "url" non-blocking mode.
 * 
 * Using SSL the connect call can return no data at all (SSL is sending and receiving 
 * info through the socket in non-blocking mode).
 * 
 * If "proto" in "URL" is http, the connection is established directly, without SSL.
 *
 **/
int w_ssl_connect(int sd, URL *url, SSL_CTX **p_ctx, SSL **p_ssl) 
{
  	OpenSSL_add_ssl_algorithms();
	SSL_load_error_strings();

   	SSL_CTX* ctx;
  	SSL*     ssl=NULL;
    const SSL_METHOD *meth;
    time_t elapsedTime;
    int ret;

    meth = SSLv23_client_method();
	ctx = SSL_CTX_new (meth);

    if (strcmp(url->proto,"https")==0) {
        // Ok, inicializa la estructura para ssl
	    ssl = SSL_new (ctx);
        if (SSL_set_fd (ssl, sd)==0) {
            printf ("SSL Connect Failure...\n");
            sprintf(url->status,"SSL Connection Error.");
            return -1;
        }

        printf ("Start SSL Handshaking...\n");

        // Start SSL Handshaking
        elapsedTime = time(NULL);
        while (1) {
            if ( (time(NULL) - elapsedTime) > 10) {
                sprintf(url->status,"Timeout (10) while establishing SSL connection.");
                return -1;
            }
            if( (ret = SSL_connect (ssl)) < 0) {
	            switch (SSL_get_error(ssl,ret)) {
                case SSL_ERROR_WANT_READ:
                    continue;
                    break;
                case SSL_ERROR_WANT_WRITE:
                    continue;
                    break;
                case SSL_ERROR_WANT_CONNECT:
                    continue;
                    break;
                case SSL_ERROR_WANT_ACCEPT:
                    continue;
                    break;
                default:
                    printf("SSL Connect failed\n");
                    sprintf(url->status,"Session SSL Error.");
		            return -1;
	            }
            } else if ( ret == 0 ) {
                printf ("SSL Connect Shutdown due to security reasons.\n");
            } else {
                break;
            }
        }

        printf ("SSL Connection Established.\n");

        // Up to this point, SSL connection is established.

    }

    (*p_ssl) = ssl;
    (*p_ctx) = ctx;

    return ret;
}


/**
 * TCP/IP Connection Establish Function
 * Open the connection considering proxy.
 *
 **/
int w_smart_connect(URL *url)
{

	int sd;
    int ret;
    time_t elapsedTime;
    int flags;
	struct sockaddr_in sa;
    struct hostent *hp;

	char buf[4096];

 
    if ( (sd = socket (AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fprintf (stderr, "Cannot create socket.\r\n");
        return -2;
    }

  	memset (&sa, '\0', sizeof(sa));

    if((hp=gethostbyname(url->proxied ? url->proxyhost : url->host))==NULL)
    {
        fprintf(stderr,"No host %s!\n",url->proxied ? url->proxyhost : url->host);
        sprintf(url->status,"No host %s.", url->proxied ? url->proxyhost : url->host);
        return -1;
    }

    /*pone la direccion del host y el tipo de direccion 
    dentro de una estructura de socket*/
    bcopy((char *)hp->h_addr,(char *)&sa.sin_addr,hp->h_length);


  	sa.sin_family      = AF_INET;
  	//sa.sin_addr.s_addr = inet_addr (url->proxied ? url->proxyhost : url->host);   /* Server IP */
  	sa.sin_port        = htons     (url->proxied ? url->proxyport : url->port);          /* Server Port number */


    printf ("Connected to %s:%d\r\n", url->proxied ? url->proxyhost : url->host, url->proxied ? url->proxyport : url->port);

  	if(connect(sd, (struct sockaddr*) &sa, sizeof(sa)) < 0)
	{
		printf("Connection failed\n");
        sprintf(url->status,"Cannot establish connection with the server.");
		return -1;
	}

    printf ("Connected to %s successfuly.\n",url->proxied ? url->proxyhost : url->host);

    // Non-Blocking Socket.
    
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

    return sd;

    // OK !
}

/**
 * SSL Handshaking and connection establishment.
 * 
 **/
int checkSSLAccess(char *urlstring,char *method, URL *url, int iTimeout, char *proxyhost, int proxyport, char *encodedProxyPwd, char *postData, char *errorWaterMark, char *expectedWaterMark)
{

    (*url) = parseURL(urlstring);
	
	int sd;
    int ret;

  	SSL_CTX* ctx;
  	SSL*     ssl=NULL;

    //char postData[4096];
    char jsession[256];

    jsession[0]='\0';
    if ( method == NULL || strcmp(method,"")==0 )
        strcpy(url->method,"GET");
    else
        strcpy(url->method,method);

    strcpy(url->status,"");

    // Set up proxy parameters.
    if (proxyhost != NULL && strcmp(proxyhost,"")!=0) {
        url->proxied = true;
        strcpy(url->proxyhost,proxyhost);
        url->proxyport=proxyport;
        strcpy(url->encodedProxyPwd,encodedProxyPwd);

        // When using proxy access some servers need the entire url in the path field.
        strcpy(url->path,urlstring);
    }

    // Connect to proxy or server.
    sd = w_smart_connect( url );

    if (sd < 0) {
        return sd;
    }

    // Proxied https, first establish the connection with the proxy and the desired destination.
    if ( url->proxied && strcmp(url->proto,"https")==0 ) {
        pre_https_proxy_dialog(sd,url->host,url->port,url->encodedProxyPwd);
    }   

    // Start SSL Handlshake
    ret = w_ssl_connect(sd,url, &ctx, &ssl);

    if (ret < 0) {
        return ret;
    }

    ret = http_dialog(sd,ssl,url,postData,jsession,iTimeout,url->encodedProxyPwd, errorWaterMark, expectedWaterMark);

    if (ssl != NULL)
        SSL_shutdown(ssl);

    //shutdown(sd,SHUT_RDWR);
    close(sd);

    if (ssl != NULL) {
        SSL_free (ssl);  
    }

    SSL_CTX_free (ctx);   

    return ret;

    // OK !
}

/**
int main(int argc, char*argv[])
{
    URL url;
    url = parseURL(argv[1]);

    printf ("Host: %s, Path: %s\r\n", url.host, url.path);
    printf ("Port: %d, Proto: %s\r\n", url.port, url.proto);

    exit(1);
}
*/



