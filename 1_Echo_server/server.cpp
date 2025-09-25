#include "config.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

// #define PORT 8000
// #define BUFFER_SIZE 1024
// #define END_STRING "end"

int main() {
	int server_fd;
	sockaddr_in addr{};
	char buffer[BUFFER_SIZE];

	// Create Socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd == -1){
		perror("socket");
		exit(1);
	}

	// Bind
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	if(bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0){
		perror("bind");
		exit(1);
	}

	// Listen	
	if(listen(server_fd, 1) < 0){
		perror("listen");
		exit(1);
	}
	
	printf("Server is listening on port %d...\n", PORT);

	// Accept Client
	int client_fd = accept(server_fd, nullptr, nullptr);
	if(client_fd < 0){
		perror("accept");
		exit(1);
	}

	// Echo Loop
	int n;
	while((n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0)) > 0){
		buffer[n] = '\0';

		if(!strcmp(buffer, END_COMMAND)){
			close(server_fd);
			close(client_fd);
			return 0;
		}

		printf("Received: %s\n", buffer);
		send(client_fd, buffer, n, 0);
	}

	close(server_fd);
	close(client_fd);
	return 0;
}