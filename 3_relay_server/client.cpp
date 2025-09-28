#include "config.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

void check(int cond, const char* msg) {
	if (cond < 0) {
		perror(msg);
		exit(1);
	}
}

int main () {
	int sock;
	sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	check(sock = socket(AF_INET, SOCK_STREAM, 0), "socket");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if(connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)), "connect");
	printf("Connected to the Server.\n");
	
	fd_set current;

	while(true) {
		FD_ZERO(&current);
		FD_SET(STDIN_FILENO, &current);
		FD_SET(sock, &current);

		check(select(sock + 1, &current, NULL, NULL, NULL), "select");

		if(FD_ISSET(STDIN_FILENO, &current)){
			if(scanf(" %[^\n]", buffer)) {
				buffer[strcspn(buffer, "\n")] = 0;
				send(sock, buffer, strlen(buffer), 0);
				if (!strcmp(buffer, END_COMMAND)) break;
			}
		}

		if(FD_ISSET(sock, &current)) {
			int n = recv(sock ,buffer, BUFFER_SIZE - 1, 0);
			if(n <= 0) {
				printf("Server closed connection.\n");
				break;
			}
			buffer[n] = '\0';
			printf("Peer: %s\n", buffer);
		}
	}

	close(sock);
	return 0;
}