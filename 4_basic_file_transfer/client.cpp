#include "config.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fstream>
#include <format>

using namespace std;

void check(int cond, const char* msg) {
	if (cond < 0) {
		perror(msg);
		exit(1);
	}
}

void show_progress(streamsize recieved, streamsize total) {
	int barWidth = 50;
	float progress = (float) recieved / total;
	int pos = barWidth * progress;

	cout<<"\r"<<BLUE<<"[";
	for (int i=0; i<barWidth; i++) {
		if (i < pos) cout<<GREEN<<"="<<RESET;
		else if (i == pos) cout<<">";
		else cout<<" ";
	}
	cout<<BLUE<<"] "<<PURPLE_BG<<format("{:.2f}", progress*100)<<"%"<<RESET<<flush;
}

int main() {
	int sock;
	sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];

	check(sock = socket(AF_INET, SOCK_STREAM, 0), "socket");

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	check(connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)), "connect");

	streamsize filesize;
	recv(sock, &filesize, sizeof(filesize), 0);
	cout<<"File size: "<<filesize<<" bytes\n";

	ofstream outfile(OUT_FILENAME, ios::binary);
	if(!outfile.is_open()){
		perror("file");
		exit(1);
	}

	streamsize recieved = 0;
	ssize_t n;
	cout<<HIDE_CURSOR;
	while(recieved < filesize) {
		check(n = recv(sock, buffer, BUFFER_SIZE, 0), "recv");

		outfile.write(buffer, n);
		recieved += n;

		show_progress(recieved, filesize);
	}
	cout<<SHOW_CURSOR;

	cout<<"\nDownload Complete.\n";

	outfile.close();
	close(sock);
	return 0;
}