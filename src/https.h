// Https processing.
#include <openssl/ssl.h>

struct URL {
    char proto[10];
    int port;
    char host[256];
    char path[1024];
    char method[6];
    long milliseconds;
    int httpstatus;
    char status[1024];
    bool proxied;
    char proxyhost[256];
    int  proxyport;
    char encodedProxyPwd[256];
};

int checkSSLAccess(char *urlstring,char *method, URL *url, int iTimeout, char *proxyhost, int proxyport, char *encodedProxyPwd, char *postData,char *errorWaterMark, char *expectedWaterMark);
int w_smart_connect(URL *url);
int w_ssl_connect(int sd, URL *url, SSL_CTX **p_ctx, SSL **p_ssl);
int http_dialog(int sd,SSL *ssl, URL *url,char *postData, char *jsession, int iTimeout,char *encodedProxyPwd, char *errorWaterMark, char *expectedWaterMark);
URL parseURL(char *urlstring);
void pre_https_proxy_dialog(int sd,char host[256],int port,char encodedProxyPwd[256]);
 

