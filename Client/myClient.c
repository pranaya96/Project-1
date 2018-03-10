/*

  CLIENT.c
  ==========
  (c) Pranaya Adhikari, 2018
  Simple TCP/IP client.

*/


#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

//#include "helper.h"           /*  Our own helper functions  */
//#include "helper.c" 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



/*  Global constants  */
#define MAX_LINE           (1000)
#define nul '\0'
#define TRUE 1

/*  Function declarations  */



/*  main()  */

int main(int argc, char *argv[]) {

    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
    char     *filePath;              /*  Holds file path           */
    char     *toFormat;              /*  Holds target file format  */
    char     *toName;                /*  Holds target file name    */
    char     *endptr;                /*  for strtol()              */


    /*  Get command line arguments  */

    if (argc != 6){
        printf("\nCommand Line Arguments not complete\n");
        exit(EXIT_FAILURE);
        return -1;
    }
    

    szAddress = argv[1] ;
    szPort = argv[2] ;
    filePath = argv[3] ;
    toFormat = argv[4];
    toName = argv[5];


    /*  Set the remote port  */

    port = strtol(szPort, &endptr, 0);
    printf("\nPort %d\n", port);
    if ( *endptr ) {
	printf("CLIENT: Invalid port supplied.\n");
	exit(EXIT_FAILURE);
    }
    
    
    /*  Create the listening socket  */
    if ( (conn_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "CLIENT: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);


    /*  Set the remote IP address  */
    if (inet_aton(szAddress, &servaddr.sin_addr) <= 0 ) {
	printf("CLIENT: Invalid remote IP address.\n");
	exit(EXIT_FAILURE);
    }
    
    /*  connect() to the remote echo server  */
    if (connect(conn_s, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
	printf("CLIENT: Error calling connect()\n");
	exit(EXIT_FAILURE);
    }

    int fileLen;
    FILE *filePtr;
    filePtr = fopen(filePath, "rb");
    if (filePtr==NULL){
        printf("Error opening file!");
        exit(1);
    }
    
    //Get the length of file
    fseek(filePtr,0, SEEK_END); 
    fileLen= ftell(filePtr); 
    printf("the length of the entire file is: %d\n",fileLen);
    fseek(filePtr, 0, SEEK_SET);
    
    //buffer to read the file content
    char fileBuffer[fileLen];
	//Read file contents into buffer
    fread(fileBuffer,1,fileLen,filePtr);

    char messageBuffer[MAX_LINE];   // to send the all the headers and data along in a buffer
    memcpy(messageBuffer,toFormat, 1); // append target type in buffer to send
    int fileNameLen;
    fileNameLen = strlen(toName); //length of a target file name
    
    //length of file name included in buffer
    memcpy(messageBuffer+1,&fileNameLen, sizeof(int));

    //printf("%d\n", messageBuffer);
    
    //copy the toFileName in buffer
    memcpy(messageBuffer+5,toName, fileNameLen); 

    //copy the length of file to send in the buffer
    memcpy(messageBuffer+5+fileNameLen, &fileLen, sizeof(int));

    //copy the file to a buffer
    memcpy(messageBuffer+5+fileNameLen+sizeof(int),fileBuffer, fileLen);


    /*  sending the size of file as a char array*/
    write(conn_s, messageBuffer, MAX_LINE);

    char recievedBuffer[100];

    
    read(conn_s, recievedBuffer, MAX_LINE-1);

    /*  Output echoed string  */
    recievedBuffer[100] = nul;

    printf("Sever response: %s\n", recievedBuffer);

    return EXIT_SUCCESS;
}
