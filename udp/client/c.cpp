// Server side C/C++ program to demonstrate Socket programming
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

#define PORT 8080
#define BUFSIZE 1024

int main()
{
	int fd;
	string msg;
	struct sockaddr_in servaddr;

	//Create socket
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	while (1)
	{
		cout << "Send to server : ";
		getline(cin, msg);
		sendto(fd, msg.c_str(), msg.length(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
		if (msg == "END")
			break;
	}

	close(fd);
	return 0;
}