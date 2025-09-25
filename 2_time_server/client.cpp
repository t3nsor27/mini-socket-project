#include "config.hpp"
#include<iostream>
#include<unistd.h>
#include<arpa/inet.h>
#include<cstring>

int main() {
	int sock;
	sockaddr_in server_addr{};
	char buffer[BUFFER_SIZE];

	// Create Socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) { perror("socket"); exit(1); }

	// Server Address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

	if(connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) { perror("connect"); exit(1); }

	int n = recv(sock, buffer, BUFFER_SIZE - 1, 0);
	if(n > 0) {
		buffer[n] = '\0';
		printf("Server time: %s", buffer);
	}

	close(sock);
	return 0;
}