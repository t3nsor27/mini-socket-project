#include "config.hpp"
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>

using namespace std;

void check(int cond, const char* msg) {
	if (cond < 0) {
		perror(msg);
		exit(1);
	}
}

int main() {
	int server_fd, client_fd;
	sockaddr_in addr{};
	char buffer[BUFFER_SIZE];
	int reuseaddr_opt = 1;

	check(server_fd = socket(AF_INET, SOCK_STREAM, 0), "socket");
	check(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_opt, sizeof(reuseaddr_opt)), "setsockpt");

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	check(bind(server_fd, (sockaddr *)&addr, sizeof(addr)), "bind");

	check(listen(server_fd, 1), "listen");
	cout<<"Server listening on port "<<PORT<<"...\n";

	check(client_fd = accept(server_fd, nullptr, nullptr), "accept");

	ifstream file(IN_FILENAME, ios::binary | ios::ate);
	if(!file.is_open()) {
		perror("file");
		close(server_fd);
		exit(1);
	}

	streamsize filesize = file.tellg();
	send(client_fd, &filesize, sizeof(filesize), 0);

	file.seekg(0, ios::beg);
	while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0) {
		send(client_fd, buffer, file.gcount(), 0);
	}

	cout<<IN_FILENAME<<" successfully sent ("<<filesize<<" bytes).\n";

	file.close();
	close(client_fd);
	close(server_fd);

	return 0;
}