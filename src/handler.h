// Signatures for handler processing.
#include <pthread.h>
#include "xslprocessor.h"

struct CONFIG {
    int timeout;
    char emaillist[256];
    // TODO:check dns max
    char mailhost[40];
    char mailport[10];
    char proxyhost[40];
    char proxyport[10];
    char proxyPwd[256];
    int alerthits;
};


void * request_handler(void *arg);
void * request_handler_test(void *arg);
void * update_thread(void *arg);

xmlDocPtr updateStatus(xmlDocPtr doc);

