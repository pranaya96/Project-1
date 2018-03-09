/*

  CLIENT.c
  ==========
  (c) Pranaya Adhikari, 2018
  Simple TCP/IP echo client.

*/


#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include "helper.h"           /*  Our own helper functions  */
#include "helper.c" 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*  Global constants  */

#define MAX_LINE           (1000)


/*  Function declarations  */

//int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort, char **filePath, char **toFormat, char **toName);


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
    //ParseCmdLine(argc, argv, &szAddress, &szPort, &filePath, &toFormat, &toName);

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


    /*  Get string to echo from user  */
    printf("Enter the string to echo: ");
    fgets(buffer, MAX_LINE, stdin);
    


    /*  Send string to echo server, and retrieve response  */
    Writeline(conn_s, buffer, strlen(buffer));
    Readline(conn_s, buffer, MAX_LINE-1);

    /*  Output echoed string  */

    printf("Echo response: %s\n", buffer);

    return EXIT_SUCCESS;
}



// int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort, char **filePath, char **toFormat, char **toName) {

//     if (argc != 6){
//         printf("\nCommand Line Arguments not complete");
//         exit(EXIT_FAILURE);
//         return -1;
//     }
    
//     else{

//         int n = 1;
//         while (n < argc ) {
//             printf("\nCommand Line Argument: %s\n", argv[n]);
//             *szAddress = argv[++n];
//             printf("\nCommand Line Argument: %s\n", argv[n]);
//             *szPort = argv[++n];
//             printf("\nCommand Line Argument: %s\n", argv[n]);
//             *filePath = argv[++n];
//             printf("\nCommand Line Argument: %s\n", argv[n]);
//             *toFormat = argv[++n];
//             printf("\nCommand Line Argument: %s\n", argv[n]);
//             *toName = argv[++n]; 
//             printf("\nCommand Line Argument: %s\n", argv[n]);  
//         }
//         exit(EXIT_SUCCESS);
//         return 0;
//     }	
// }
