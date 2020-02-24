// Server side C/C++ program to demonstrate Socket programming
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "quic-structs.cc"
using namespace std;

#define PORT 8080
#define BUFSIZE 1024

int main()
{
	int fd;
	int reclen;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	char buffer[BUFSIZE];

	//Create socket
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	if (bind(fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	cout << "Waiting on PORT " << PORT << endl;
	while (1)
	{
		reclen = recvfrom(fd, (char *)buffer, BUFSIZE, 0, (struct sockaddr *)&cliaddr, &addrlen);
		if (reclen > 0)
		{
			buffer[reclen] = '\0';
			QUICPacket *recvdpacket = new QUICPacket(buffer);
			cout << "Client : [" << recvdpacket->Header->ConnectionID << "][" << recvdpacket->Header->SequenceNumber << "]:" << recvdpacket->Message << endl;
			if (strcmp("END", buffer) == 0)
			{
				break;
			}
		}
	}

	close(fd);
	return 0;
}