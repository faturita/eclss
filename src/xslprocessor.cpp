/******************************************************************************
 * https.cpp
 *
 * XSL Processor Bridge.
 *
 * 
 * Version 1.0.0 (October 17 2008)
 *
 * THIS CODE IS FURNISHED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *
 *****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "xslprocessor.h"

extern int xmlLoadExtDtdDefaultValue;

void newXML() 
{
    xmlDocPtr doc;
    xmlNodePtr node;

    doc = xmlNewDoc((const xmlChar *)"1.0");

    node = xmlNewNode((xmlNs *)NULL,(const xmlChar *)"document");

    xmlDocSetRootElement(doc,node);

    xmlNodePtr application = xmlNewNode((xmlNs *)NULL,(const xmlChar *)"application");

    xmlSetProp(application,(const xmlChar *)"name",(const xmlChar *)"");
    xmlSetProp(application,(const xmlChar *)"url",(const xmlChar *)"");

    xmlSetProp(application,(const xmlChar *)"status",(const xmlChar *)"Ok");


    xmlAddChild(node,  application);

}

int xsltProcess(int fd, xmlDocPtr doc, char *xslFile)
{
	int i;
	const char *params[16 + 1];
	int nbparams = 0;
	xsltStylesheetPtr cur = NULL;
	xmlDocPtr res;

	params[nbparams] = NULL;
	
    xmlSubstituteEntitiesDefault(1);

	xmlLoadExtDtdDefaultValue = 1;
    printf ("parsing stylesheet\n");
	cur = xsltParseStylesheetFile((const xmlChar *)xslFile);
	
    //printf ("Transforming...\n");
	res = xsltApplyStylesheet(cur, doc, params);

    //printf ("Saving output to fd\n");

    // This code allows to write extra output to fd, because
    // of a bug presented in xsltSaveResultToFd when the fd is 
    // not longer valid.

    char buf[20];
    int val=0;
    sprintf (buf,"\r\n");

    val = write ( fd, buf, strlen(buf));

    if (val<0) {
        printf ("Broken pipe...\n");
        return -1;
    }

    
    if ( xsltSaveResultToFd(fd, res, cur) == -1)  {
        printf ("Error processing xsl.....\n");
        return -1;
    }
    
    //printf ("Releasing memory...\n");

    xsltFreeStylesheet(cur);
	xmlFreeDoc(res);
	//xmlFreeDoc(doc);

    xsltCleanupGlobals();
    //xmlCleanupParser();

    return(0);

}
int xsltProcess(int fd, char *xmlFile, char *xslFile) 
{
	int i;
	const char *params[16 + 1];
	int nbparams = 0;
	xsltStylesheetPtr cur = NULL;
	xmlDocPtr doc, res;

	params[nbparams] = NULL;
	
    xmlSubstituteEntitiesDefault(1);

	xmlLoadExtDtdDefaultValue = 1;
	cur = xsltParseStylesheetFile((const xmlChar *)xslFile);
	
	doc = xmlParseFile(xmlFile);
	res = xsltApplyStylesheet(cur, doc, params);
	if ( xsltSaveResultToFd(fd, res, cur) == -1) 
        return -1;

	xsltFreeStylesheet(cur);
	xmlFreeDoc(res);
	//xmlFreeDoc(doc);

    //xsltCleanupGlobals();
    //xmlCleanupParser();

    return(0);
}

