#include "config.hpp"
#include<iostream>
#include<unistd.h>
#include<arpa/inet.h>
#include<cstring>
#include<time.h>

int main() {
	int server_fd, client_fd;
	sockaddr_in addr{};
	char buffer[BUFFER_SIZE];

	// Create Socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0) {perror("socket"); exit(1);}

	// Bind
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	if(bind(server_fd, (sockaddr *)&addr, sizeof(addr)) < 0) {perror("bind"); exit(1);}

	// Listen
	if(listen(server_fd, 5) < 0) {perror("listen"); exit(1);}
	printf("Server lsitening on port %d...\n", PORT);

	// Accept Clients
	while(1) {
		client_fd = accept(server_fd, nullptr, nullptr);
		if(client_fd < 0) { perror("accept"); continue; }

		time_t curr_time = time(NULL);
		struct tm *tm_info = localtime(&curr_time);
		if(tm_info == NULL) { perror("localtime"); exit(1); }
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S\n", tm_info);

		send(client_fd, buffer, strlen(buffer), 0);

		close(client_fd);
	}

	close(server_fd);
	return 0;
}