// Server side C/C++ program to demonstrate Socket programming
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "quic-structs.cc"
using namespace std;

#define SERVER_PORT 8080
#define CLIENT_PORT 9080
#define BUFSIZE 1024

map<int, map<int, string>> dataStore;

int main()
{
	string message = "Client Program!";
  	cout << message << endl;
	  
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
	servaddr.sin_port = htons(SERVER_PORT);

	cliaddr.sin_family = AF_INET; // IPv4
	cliaddr.sin_addr.s_addr = INADDR_ANY;
	cliaddr.sin_port = htons(CLIENT_PORT);

	if (bind(fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	cout << "Server bound to Port: " << SERVER_PORT << endl;
	while (1)
	{
		reclen = recvfrom(fd, (char *)buffer, BUFSIZE, 0, (struct sockaddr *)&cliaddr, &addrlen);
		if (reclen > 0)
		{
			buffer[reclen] = '\0';
			QUICPacket *recvdpacket = new QUICPacket(buffer);
			if (strcmp("END", recvdpacket->message.c_str()) == 0){
				cout << "Terminating connection with the server" << endl;
				break;
			}
			cout << "Received: [" << recvdpacket->header->streamID << "][" << recvdpacket->header->sequenceNumber << "]: " << recvdpacket->message << endl;
			dataStore[recvdpacket->header->streamID].insert(pair<int, string>(recvdpacket->header->sequenceNumber, recvdpacket->message));
		}
	}

	for (auto oitr = dataStore.begin(); oitr != dataStore.end(); oitr++) { 
		cout << "StreamID " << oitr->first << ": ";
        for (auto iitr = oitr->second.begin(); iitr != oitr->second.end(); iitr++) { 
            cout << iitr->second;
        }
		cout << endl;
    }

	close(fd);
	return 0;
}