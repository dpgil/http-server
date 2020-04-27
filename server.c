#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

char *reply = 
"HTTP/1.1 200 OK\n"
"Date: Sun, 26 Apr 2020 20:08:00 PST\n"
"Server: Apache/2.2.3\n"
"Last-Modified: Sun, 26 Apr 2020 20:08:58 GMT\n"
"Content-Type: text/html\n"
"Content-Length: 15\n"
"Accept-Ranges: bytes\n"
"Connection: close\n"
"\n"
"hi daniel";

int main() {
	int port = 8888;

	// Create a stream socket with AF_INET protocol family
	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket < 0) {
		printf("Error creating socket\n");
		return 1;
	}

	// Bind the socket to port 8888
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	if (bind(listen_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		printf("Error binding socket to port %d\n", port);
		return 2;
	}

	// Listen on the socket
	if (listen(listen_socket, 1) < 0) {
		printf("Error listening on socket\n");
		return 3;
	}

	printf("Listening on port %d\n", port);
	struct sockaddr_in client_addr;

	// Accept connections forever
	while (1) {
		socklen_t addr_len = sizeof(client_addr);
		int client = accept(listen_socket, (struct sockaddr *) &client_addr, &addr_len);
		if (client < 0) {
			printf("Error accepting connection\n");
			return 4;
		}
		
		// Send a dummy response to the client
		// TODO: This should actually serve files
		send(client, reply, strlen(reply), 0);
		shutdown(client, SHUT_WR);

		// Wait for client response
		char *buf = malloc(65535);
		int rcvd = recv(client, buf, 65535, 0);
		
		// Close connection with client
		close(client);
	}	

	return 0;
}
