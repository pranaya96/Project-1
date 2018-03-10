/*
  SERVER.c
  ==========
  (c) Pranaya Adhikair, 2018
  Simple TCP/IP echo server.
*/


#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <string.h>
#include "helper.h"           /*  our own helper functions  */
#include "helper.c"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>



/*  Global constants  */
#define nul '\0'
#define TRUE 1
#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)
#define CONFIRM_LINE       (200)



int main(int argc, char *argv[]) {
    int       list_s;                /*  listening socket          */
    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char      *endptr;                /*  for strtol()              */
    int status;                       /*to send back the status 'success' or 'format error' to client */

    /*  Get port number from the command line, and
        set to default port if no arguments were supplied  */
    if ( argc == 2 ) {
	port = strtol(argv[1], &endptr, 0);
	if ( *endptr ) {
        fprintf(stderr, "SERVER: Invalid port number.\n");
	    exit(EXIT_FAILURE);
    }
    }
    else if ( argc < 2 ) {
	port = ECHO_PORT;
    }
    else {
	fprintf(stderr, "SERVER: Invalid arguments.\n");
	exit(EXIT_FAILURE);
    }

	
    /*  Create the listening socket  */
    if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "SERVER: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);


    /*  Bind our socket addresss to the 
	listening socket, and call listen()  */
    if (bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
        fprintf(stderr, "SERVER: Error calling bind()\n");
        exit(EXIT_FAILURE);
    }

    if (listen(list_s, LISTENQ) < 0 ) {
	fprintf(stderr, "SERVER: Error calling listen()\n");
	exit(EXIT_FAILURE);
    }

    
    /*  Enter an infinite loop to respond
        to client requests and echo input  */

    while ( 1 ) {

	/*  Wait for a connection, then accept() it  */

	if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling accept()\n");
	    exit(EXIT_FAILURE);
	}
	/*  Retrieve an input line from the connected socket
	    then simply write it back to the same socket. */
    read(conn_s, buffer, MAX_LINE);
    

    //get the format type
    char formatType[2];
    formatType[0] = buffer[0];
    formatType[1] = nul;


    if (strstr("0123", formatType) == NULL) {
        printf("Format Type not correct");
        status = -1; // invalid formatType can't save
        exit(EXIT_FAILURE);
    }

    
    //length of fileName in integer
    char fileNameLenInt[4];
    memcpy(fileNameLenInt, buffer+1, sizeof(int));
    
    int lenOfFileName = *fileNameLenInt;

   
    // get name of target fileName
    char targetFileName[lenOfFileName+1];
    memcpy(targetFileName, buffer+5, lenOfFileName);
    targetFileName[lenOfFileName] = nul;

   

    //get the length of File
    char fileLengthInt[4];
    memcpy(fileLengthInt, buffer+5+lenOfFileName, 4);
    int lenOfFile = *fileLengthInt;

    

   

    //get the actual data in the file now
    char fileContent[lenOfFile];
    memcpy(fileContent,buffer+5+lenOfFileName+4, lenOfFile);

    /*file pointer to open a file in binary append(write) mode*/
    FILE *targetFilePtr;
    targetFilePtr = fopen(targetFileName, "ab+");
    int pointerPosition;
    pointerPosition = 0;
    while (pointerPosition < lenOfFile-1){
        
        if (fileContent[pointerPosition] == 0) {
            printf("\nNums of Type0:\n");
            pointerPosition = readTypeZero(fileContent, pointerPosition, targetFilePtr, formatType);
        }
        if (fileContent[pointerPosition]== 1){
            printf("\nNums of Type1:\n");
            pointerPosition = readTypeOne(fileContent, pointerPosition, targetFilePtr, formatType);
        }
    }



    fclose(targetFilePtr);

    /* char array to send the confirmation message back to the client*/
    char confirmationMessage[CONFIRM_LINE];
    if (status == -1){
        strncpy(confirmationMessage,"Format Error", CONFIRM_LINE);
    }
    else{
        strncpy(confirmationMessage,"File sucessfully translated and saved in server.", CONFIRM_LINE);

    }
   
   
   
    confirmationMessage[CONFIRM_LINE] = nul;

    /*write confirmation message to the socket*/
    write(conn_s, confirmationMessage, CONFIRM_LINE);

	/*  Close the connected socket  */
	if ( close(conn_s) < 0 ) {
	    fprintf(stderr, "ECHOSERV: Error calling close()\n");
	    exit(EXIT_FAILURE);
    }
    exit(0);
    }
   
}
