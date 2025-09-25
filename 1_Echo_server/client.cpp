#include "config.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
	int sock;
	sockaddr_in server_addr{};
	char buffer[BUFFER_SIZE];

	// Create Socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}

	// Server Address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

	// Connect to Server
	if(connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("connect");
		exit(1);
	}

	printf("Connected to server.\n");

	// Send & Receive
	while(fgets(buffer, BUFFER_SIZE - 1, stdin) != NULL){
		buffer[strcspn(buffer, "\n")] = 0;

		send(sock, buffer, strlen(buffer), 0);

		if(!strcmp(buffer, END_COMMAND)){
			close(sock);
			return 0;
		}

		int n = recv(sock, buffer, BUFFER_SIZE - 1, 0);
		buffer[n] = '\0';
		printf("Echo: %s\n", buffer);
	}
	
	close(sock);
	return 0;
}