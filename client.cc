#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "quic-structs.cc"
using namespace std;

#define SERVER_PORT 8080
#define CLIENT_PORT 9080
#define BUFSIZE 1024

int streamid = 0;

vector<QUICPacket> SplitMessage(string msg){
	vector<QUICPacket> results;
	int seqnum = 0;
	for (int i = 0; i < msg.length(); ) {
		QUICPacket *p = new QUICPacket(187, streamid, seqnum++, msg.substr(i, 3));
		results.push_back(*p);
		i+=3;
	}
	random_shuffle(results.begin(), results.end());
	streamid++;
	return results;
}

void SendMessageToServer(vector<QUICPacket> allPackets, int fd, sockaddr_in servaddr, int mid) {
	for(auto i = allPackets.begin(); i != allPackets.end(); ++i){
		string sendbuffer = (*i).SerializePacket();
		sendto(fd, sendbuffer.c_str(), sizeof(sendbuffer), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
	}
	cout << "Completed Message " << mid << endl;
}

int main() {
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

	if (bind(fd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	cout << "Client bound to Port: " << SERVER_PORT << endl;

	string messages[7] = {
		"Hello there!",
		"Welcome to this QUIC Demo",
		"This implementation is done in C++",
		"Note that this is a work in progress",
		"Thank you for your time",
		"Have a good day (:",
		"END"
	};

	vector<thread> threads;
	for (int i = 0; i < 7; i++) {
		cout << "Sending to server: " << messages[i] << endl;
		vector<QUICPacket> allPackets = SplitMessage(messages[i]);
		threads.push_back(thread(SendMessageToServer, allPackets, fd, servaddr, i));
	}

	for (auto& t : threads)
		t.join();

	close(fd);
	return 0;
}