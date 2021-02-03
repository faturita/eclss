/******************************************************************************
*  
*  Autor: Daniel Lerch
*  e-mail: lerch@ya.com
*
*  References:
*  Network security with OpenSSL (O'Reilly)
*
*  Compile:
*  $ gcc base64.c -o base64 -lssl
*
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

unsigned char *base64_encode (unsigned char *buffer, unsigned int len) {

  unsigned char *ret = (unsigned char *) malloc ((((len+2)/3)*4)+1);
  //EVP_EncodeBlock (ret, buffer, len);
  ret[(((len+2)/3)*4)] = 0;
  return ret;
}

unsigned char *base64_decode (unsigned char *buffer, unsigned int len) {

  unsigned char *ret = (unsigned char *) malloc ((((len+2)/3)*4)+1);
  //EVP_DecodeBlock (ret, buffer, len);
  ret[(((len+2)/3)*4)] = 0;
  return ret;
}

void usage (char *progname) {

   printf ("Usage: %s [ encode | decode ] Text\n\n", progname); 
   exit(0); 
}

int main_test (int argc, char *argv[])
{
   int i;

   /* Without parameters */
   if (argc < 3) usage(argv[0]);

   if (argc == 3) {

      /* Verificamos los algoritmos */
      if (strcmp(argv[1], "encode")  == 0) 
         printf ("%s\n", base64_encode ((unsigned char *)argv[2], strlen(argv[2])));

      if (strcmp(argv[1], "decode") == 0) 
         printf ("%s\n", base64_decode ((unsigned char *)argv[2], strlen(argv[2])));

   }
   else  usage(argv[0]);

   return 0;
}

