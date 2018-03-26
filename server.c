#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Global constants
#define ECHO_PORT (5000)
#define MAX_LINE (1000)

int main() {
	int list_s;						// listening socket
	int bind_s;
	int conn_s; 					// connections socket
	short int port;					// port number
	struct sockaddr_in servaddr;	// socket address structures
	char buffer[MAX_LINE];			// character buffer
	char *endptr;				// for strtol()
	
	// creating the listening socket
	list_s = socket(AF_INET, SOCK_STREAM, 0);
	
	// checking for the error
	if (list_s < 0) {
		printf("ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}

	// set all the bytes is socket structure to 0
	memset(&servaddr, 0, sizeof(servaddr));
	
	// filling in the relative data members 
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	// bind our socket address to the listening socket, and call listen()
	bind_s = bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
	// checking if the socket is binded properly
	if (bind_s < 0) {
		printf("ECHOSERV: Error calling bind()\n");
		exit(EXIT_FAILURE);
	}

	if (listen(list_s, LISTENQ) < 0) {
		fprintf(stderr, "ECHOSERV: Error calling listen()\n");
		exit(EXIT_FAILURE);
	}
	
	printf("TCP Client starting...");
	
	// TODO: need change here 
	FILE* file = "practic_project_test_file_1";
	
	// enter an infinte loop to respond to client requests and echo input
	while (1) {
		if ((conn_s = accept(list_s, NULL, NULL) ) < 0) {
			fprintf(stderr, "ECHOSERV: Error calling accpet().\n");
			exit(EXIT_FAILURE);
		}
		Readline(conn_s, buffer, MAX_LINE - 1);
		Writeline(conn_s, buffer, strlen(buffer));
	
		// close the connect socket
		if (close(conn_s) < 0) {
			fprintf(stderr, "ECHOSERV: Error calling close()\n");
			exit(EXIT_FAILURE);
		}
	}
}
