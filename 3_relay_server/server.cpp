#include "config.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

void check(int cond, const char* msg) {
	if (cond < 0) {
		perror(msg);
		exit(1);
	}
}

int main() {
	int server_fd, client1_fd, client2_fd;
	sockaddr_in addr{};
	char buffer[BUFFER_SIZE];
	int addr_reuse_opt = 1;

	check(server_fd = socket(AF_INET, SOCK_STREAM, 0), "socket");
	check(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &addr_reuse_opt, sizeof(addr_reuse_opt)), "setsocketop");

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	check(bind(server_fd, (sockaddr *)&addr, sizeof(addr)), "bind");

	check(listen(server_fd, 2), "listen");
	printf("Server is listening on port %d...\n", PORT);

	if(client1_fd = accept(server_fd, nullptr, nullptr), "accept client 1");
	printf("Client 1 connected.\n");

	if(client2_fd = accept(server_fd, nullptr, nullptr), "accept client 2");
	printf("Client 2 connected.\n");

	fd_set current;
	int max_fd = client1_fd > client2_fd ? client1_fd : client2_fd;

	while(true) {
		FD_ZERO(&current);
		FD_SET(client1_fd, &current);
		FD_SET(client2_fd, &current);
		check( select(max_fd + 1, &current, NULL, NULL, NULL), "select");

		if(FD_ISSET(client1_fd, &current)) {
			int n = recv(client1_fd, buffer, BUFFER_SIZE - 1, 0);
			if (n <= 0) {
				printf("Client 1 disconnected\n");
				break;
			}

			buffer[n] = '\0';
			if(!strcmp(buffer, END_COMMAND)) break;
			send(client2_fd, buffer, n, 0);
		}

		if(FD_ISSET(client2_fd, &current)) {
			int n = recv(client2_fd, buffer, BUFFER_SIZE - 1, 0);
			if (n <= 0) {
				printf("Client 1 disconnected\n");
				break;
			}

			buffer[n] = '\0';
			if(!strcmp(buffer, END_COMMAND)) break;
			send(client1_fd, buffer, n, 0);
		}
	}

	close(client1_fd);
	close(client2_fd);
	close(server_fd);
	return 0;
}