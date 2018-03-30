#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Global constants
#define MAX_LINE (1025)


int main() {
	int conn_socket;						// listening socket
	int bind_socket;						// binding socket
	short int port;							// port number
	// socket address structures
	// particular for of the sockaddr used for TCP/IP addresses
	struct sockaddr_in servaddr;
	char buffer[MAX_LINE];					// character buffer
	int filesize;							// size of the file to send to server
	FILE* file_to_send;						// file to send
	char* ip_address;						// holds remote IP address
	char* port_num;							// holds remote port number
	char* endptr;							// for strtol()

	// setting the remote port
	port = strtol(port_num, &endptr, 0);
	if (*endptr) {
		printf("ECHOCLIENT: Invalid port supplied.\n");
		exit(EXIT_FAILURE);
	}
	
	// creating the listening socket
	conn_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	// checking for the error
	if (conn_socket < 0) {
		printf("ECHOCLIENT: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}

	// set all the bytes is socket structure to 0
	memset(&servaddr, '0', sizeof(servaddr));
	
	// filling in the relative data members 
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// setting the remote IP address
	if ( inet_aton(ip_address, &servaddr.sin_addr ) < 0 ) {
		printf("ECHOCLIENT: Invalid remote IP address.\n");
		exit(EXIT_FAILURE);
	}

	// bind our socket address to the listening socket, and call listen()
	bind_socket = bind(conn_socket, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	printf("TCP Client establishing connection...");
	
	// establishing connection to the server 
	// checking for error
	if (connect(conn_socket, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		printf("ECHOSERV: Error establishing connection.\n");
		exit(EXIT_FAILURE);
	}
	
	// TODO: need change here 
	file_to_send = fopen("practic_project_test_file_1","rb");
	// moving the pointer to the end of the file
	fseek(file_to_send, 0, SEEK_END);
	// finding the size of the file
	// ftell returns current value of the position indicator of the stream
	filesize = ftell(file_to_send);
	// resetting the position indicator to the original state
	rewind(file_to_send);
	
	// read data file to buffer
	fread(buffer,1,filesize, file_to_send);
	
	// writing the data of the file to the socket
	// if error in writing show error message
	if (write(conn_socket, buffer, filesize) < 0) {
		printf("ECHOCLIENT: Error writing to the socket.\n");
	}
	// if error in reading show error message
	if (read(conn_socket, buffer, filesize) < 0) 
		printf("ECHOCLIENT: Error reading from the socket.\n");
	
	return EXIT_SUCCESS;
}

// get 
