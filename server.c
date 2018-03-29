#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	// server socket
	int server_socket;
	
	// creating server socket
	server_socket = socket(AF_NET, SOCK_STREAM, 0);
	
	// checking for erro
	if (server_socket < 0) {
		printf("ECHOSERV: Error creating a server socket.\n");
		exit(EXIT_FAILURE);
	}
	
	// socket address
	struct sockaddr_in serv_addr;
	
	// initializing every member of socket_addr with 0
	memset(&serv_addr, 0, sizeof(serv_addr);
	
	serv_addr.sin_family = AF_NET;
	serv_addr.sin_port = htons(PORT);
	
	list_s = socket(AF_NET, SOCK_STREAM, 0);	if (list_s < 0) {
		printf("ECHOSERV: Error creating listening socket.\n");
		exit(EXIT_FAILURE);
	}
	
	if (connect(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("ECHOSERV: Error connecting socket.\n");
		exit(EXIT_FAILURE);
	}
	
	
	return 0;
}