#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "quic-structs.cc"
using namespace std;

#define SERVER_PORT 8080
#define BUFSIZE 1024

int seqnum = 0;

vector<QUICPacket> SplitMessage(string msg){
	vector<QUICPacket> results;
	for (int i = 0; i < msg.length(); ) {
		QUICPacket *p = new QUICPacket(187, seqnum++, msg.substr(i, 2));
		results.push_back(*p);
		i+=2;
	}
	return results;
}

int main() {
	string message = "Client Program!";
  	cout << message << endl;

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
	servaddr.sin_port = htons(SERVER_PORT);

	while (1)
	{
		cout << "Send to server : ";
		getline(cin, msg);
		vector<QUICPacket> allPackets = SplitMessage(msg);
		for(auto i = allPackets.begin(); i != allPackets.end(); ++i){
			string sendbuffer = (*i).SerializePacket();
			// cout << sendbuffer << " : " << sizeof(sendbuffer);
			sendto(fd, sendbuffer.c_str(), sizeof(sendbuffer), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
		}
		if (msg == "END")
			break;
	}

	close(fd);
	return 0;
}