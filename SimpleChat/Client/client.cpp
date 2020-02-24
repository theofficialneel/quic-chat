// Client side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../QUIC/quic.h"

#define PORT 8080
#define BUFSIZE 1024

vector<QUICPacket*> quicArray(string s) {
    vector<QUICPacket*> result;
    int i = 0;
    while(i < s.length()) {
        cout<<s.substr(i, 3)<<endl;
        QUICPacket *q = new QUICPacket(i, s.substr(i, 3));
        result.push_back(q);
        i += 3;
    }

    return result;
}

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
        vector<QUICPacket*> packetsArray = quicArray(msg);
		// sendto(fd, msg.c_str(), msg.length(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        for(auto i = packetsArray.begin(); i != packetsArray.end(); ++i) {
            string serialMsg = (*i)->serialString();
            sendto(fd, serialMsg.c_str(), serialMsg.length(), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        }

		if (msg == "END")
			break;
	}

	close(fd);
	return 0;
}