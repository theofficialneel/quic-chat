#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
using namespace std;

class QUICHeader {
public:
	int SequenceNumber;
    int ConnectionID;
    int Version;

	QUICHeader(int cid, int seq) {
		ConnectionID = cid;
		SequenceNumber = seq;
	}
};

class QUICPacket {
public:
	QUICHeader *Header;
	string Message;

	QUICPacket(int cid, int seq, string msg) {
		Header = new QUICHeader(cid, seq);
		Message = msg;
	}

	QUICPacket(string serializedbuffer) {
		vector<string> result;
		boost::split(result, serializedbuffer, boost::is_any_of(";"));
		Header = new QUICHeader(atoi(result[0].c_str()), atoi(result[1].c_str()));
		Message = result[2];
	}

	string SerializePacket() {
		string result = "";
		result += to_string(this->Header->ConnectionID);
		result += ";";
		result += to_string(this->Header->SequenceNumber);
		result += ";";
		result += this->Message;
		result += ";";
		return result;
	}

};