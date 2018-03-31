#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include "helper.h"           /*  Our own helper functions  */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*  Global constants  */
#define MAX_LINE           (1000)

/*  Function declarations  */
int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort);


/*  main()  */
int main(int argc, char *argv[]) {

    int       conn_s;                /*  connection socket         */
    short int port;                  /*  port number               */
    struct    sockaddr_in servaddr;  /*  socket address structure  */
    char      buffer[MAX_LINE];      /*  character buffer          */
    char     *szAddress;             /*  Holds remote IP address   */
    char     *szPort;                /*  Holds remote port         */
    char     *endptr;                /*  for strtol()              */
	unsigned long filesize;			 // size of the file
	unsigned long received_filesize;
	FILE* file_to_send;


    /*  Get command line arguments  */
    ParseCmdLine(argc, argv, &szAddress, &szPort);


    /*  Set the remote port  */
    port = strtol(szPort, &endptr, 0);
    if ( *endptr ) {
	printf("ECHOCLNT: Invalid port supplied.\n");
	exit(EXIT_FAILURE);
    }
	

    /*  Create the listening socket  */

    if ( (conn_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "ECHOCLNT: Error creating listening socket.\n");
	exit(EXIT_FAILURE);
    }


    /*  Set all bytes in socket address structure to
        zero, and fill in the relevant data members   */

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);


    /*  Set the remote IP address  */

    if ( inet_aton(szAddress, &servaddr.sin_addr) <= 0 ) {
	printf("ECHOCLNT: Invalid remote IP address.\n");
	exit(EXIT_FAILURE);
    }

    
    /*  connect() to the remote echo server  */

    if ( connect(conn_s, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
	printf("ECHOCLNT: Error calling connect()\n");
	exit(EXIT_FAILURE);
    }
	
	// TODO: need change here 
	file_to_send = fopen("inputFile","rb");
	if (file_to_send == NULL) {
		printf("Failed to open the file.\n");
	}
	// moving the pointer to the end of the file
	fseek(file_to_send, 0, SEEK_END);
	
	// finding the size of the file
	// ftell returns current value of the position indicator of the stream
	filesize = ftell(file_to_send);
	// resetting the position indicator to the original state
	rewind(file_to_send);
	
	// read data file to buffer
	fread(buffer,1,filesize, file_to_send);
	printf("Filesize in client: %lu\n", filesize);
	printf("Sending file size to the server.\n");
	// Send file size to server
	Writeline(conn_s, &filesize, sizeof(long));
	printf("Receiving file size from the server.\n");
	// receive file size from the server
	Readline(conn_s, &received_filesize, sizeof(long));
	
	// Send data to echo server, and retrieve response
	printf("Sending the buffer to the server.\n");
    Writeline(conn_s, buffer, filesize);
	printf("Finished sending buffer to the server.\n");
    // Readline(conn_s, buffer, filesize);
	// printf("Finished reading response from the server.\n");
	
	// Getting the error message from the server
	int errorMessage = -1;
	Readline(conn_s, &errorMessage, sizeof(int));
	printf("Error message: %d\n", errorMessage);
    //  Output echoed string
    // printf("Echo response: %s\n", buffer);

    return EXIT_SUCCESS;
}

int ParseCmdLine(int argc, char *argv[], char **szAddress, char **szPort) {
    int n = 1;
    while ( n < argc ) {
	if ( !strncmp(argv[n], "-a", 2) || !strncmp(argv[n], "-A", 2) ) {
	    *szAddress = argv[++n];
	}
	else if ( !strncmp(argv[n], "-p", 2) || !strncmp(argv[n], "-P", 2) ) {
	    *szPort = argv[++n];
	}
	else if ( !strncmp(argv[n], "-h", 2) || !strncmp(argv[n], "-H", 2) ) {
	    printf("Usage:\n\n");
	    printf("    timeclnt -a (remote IP) -p (remote port)\n\n");
	    exit(EXIT_SUCCESS);
	}
	++n;
    }

    return 0;
}

