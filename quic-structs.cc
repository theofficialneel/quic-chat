#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
using namespace std;

class QUICHeader {
public:
	int sequenceNumber;
    int connectionID;
	int streamID;

	QUICHeader(int cid, int sid, int seq) {
		connectionID = cid;
		sequenceNumber = seq;
		streamID = sid;
	}
};

class QUICPacket {
public:
	QUICHeader *header;
	string message;

	QUICPacket(int cid, int sid, int seq, string msg) {
		header = new QUICHeader(cid, sid, seq);
		message = msg;
	}

	QUICPacket(string serializedbuffer) {
		vector<string> result;
		boost::split(result, serializedbuffer, boost::is_any_of(";"));
		header = new QUICHeader(atoi(result[0].c_str()), atoi(result[1].c_str()), atoi(result[2].c_str()));
		message = result[3];
	}

	string SerializePacket() {
		string result = "";
		result += to_string(this->header->connectionID);
		result += ";";
		result += to_string(this->header->streamID);
		result += ";";
		result += to_string(this->header->sequenceNumber);
		result += ";";
		result += this->message;
		result += ";";
		return result;
	}

};