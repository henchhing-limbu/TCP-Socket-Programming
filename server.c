
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ECHO_PORT (2002)
#define MAX_LINE (1025)
#define LISTENQ (1024)

int main() {
	// listening socket
	int list_socket;
	
	// socket address structure
	struct sockaddr_in serv_addr;
	
	char buffer[MAX_LINE];
	// TODO: Change needed here
	short int port = ECHO_PORT;
	
	// creating listening socket
	list_socket = socket(AF_NET, SOCK_STREAM, 0);
	
	// checking for error
	if (list_socket < 0) {
		printf("ECHOSERV: Error creating a server socket.\n");
		exit(EXIT_FAILURE);
	}
	
	// initializing every member of socket_addr with 0
	memset(&serv_addr, '0', sizeof(serv_addr));
	
	serv_addr.sin_family = AF_NET;
	serv_addr.sin_port = htons(port);
	// assigning a random address
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// binding the socket address to the listening socket
	if ( bind(list_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) {
		printf("ECHOSERV: Error calling bind().\n");
		exit(EXIT_FAILURE);
	}
	// listening to the socket
	if (listen(list_socket, LISTENQ) < 0) {
		printf("ECHOSERV: Error listening to the socket.\n");
		exit(EXIT_FAILURE);
	}
	
	int connection_socket;
	// enter an infinte loop to respond to client requests
	while (1) {
		// accepting a connection
		connection_socket = accept(list_socket, NULL, NULL);
		if ( connection_socket  < 0) {
			printf("ECHOSERV: Error calling accpet().\n");
			exit(EXIT_FAILURE);
		}
		// Readline(connection_socket, buffer, MAX_LINE - 1);
		// Writeline(connection_socket, buffer, strlen(buffer));
	
		// close the connect socket
		if (close(connection_socket) < 0) {
			printf("ECHOSERV: Error calling close()\n");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}