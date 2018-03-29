#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Global constants
#define ECHO_PORT (5000)
#define MAX_LINE (1025)

int main() {
	int list_socket;						// listening socket
	int bind_socket;						// binding socket
	int connection_socket; 					// connections socket
	short int port;							// port number
	// socket address structures
	// particular for of the sockaddr used for TCP/IP addresses
	struct sockaddr_in servaddr;
	char buffer[MAX_LINE];					// character buffer
	char *endptr;							// for strtol()
	int filesize;							// size of the file to send to server
	FILE* file_to_send;						// file to send
	
	// creating the listening socket
	list_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	// checking for the error
	if (list_socket < 0) {
		printf("ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}

	// set all the bytes is socket structure to 0
	memset(&servaddr, 0, sizeof(servaddr));
	
	// filling in the relative data members 
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind our socket address to the listening socket, and call listen()
	bind_socket = bind(list_socket, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	// checking if the socket is binded properly
	if (bind_socket < 0) {
		printf("ECHOSERV: Error calling bind()\n");
		exit(EXIT_FAILURE);
	}
	
	// instructs TCP protocol implementation to listen for connections
	if (listen(list_socket, LISTENQ) < 0) {
		printf("ECHOSERV: Error calling listen()\n");
		exit(EXIT_FAILURE);
	}
	
	printf("TCP Client establishing connection...");
	
	// establishing connection to the server 
	connection_socket = connect(list_socket, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	// checking for error
	if (connection_socket < 0) {
		printf("ECHOSERV: Error establishing connection.\n");
		exit(EXIT_FAILURE);
	}
	
	// TODO: need change here 
	file_to_send = fopen("practic_project_test_file_1");
	// moving the pointer to the end of the file
	fseek(file_to_send, 0, SEEK_END);
	// finding the size of the file
	// ftell returns current value of the position indicator of the stream
	filesize = ftell(file_to_send);
	// resetting the position indicator to the original state
	rewind(file_to_send);
	
	// read data from to buffer
	fread(buffer,1,filsize, file_to_send);
	
	// writing the data of the file to the socket
	if (write(list_socket, buffer, filesize) < 0) {
		printf("ECHOSERV: Error writing to the socket.\n");
	}
	if (read(list_socket, buffer, filesize) < 0) 
		printf("ECHOSERV: Error reading from the socket.\n");
	
	
	
	/*
	// enter an infinte loop to respond to client requests
	while (1) {
		connection_socket = accept(list_socket, NULL, NULL);
		if ( connection_socket ) < 0) {
			printf("ECHOSERV: Error calling accpet().\n");
			exit(EXIT_FAILURE);
		}
		Readline(connection_socket, buffer, MAX_LINE - 1);
		Writeline(connection_socket, buffer, strlen(buffer));
	
		// close the connect socket
		if (close(connection_socket) < 0) {
			fprintf("ECHOSERV: Error calling close()\n");
			exit(EXIT_FAILURE);
		}
	}
	*/
	return 0;
}

// get 
