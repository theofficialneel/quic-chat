#include <bits/stdc++.h>
using namespace std;

class QUICHeader {
	int _sequenceNumber;
    int _connectionID;
    int _version;

public:
	QUICHeader(int seq) {
		_sequenceNumber = seq;
	}

    int getSeqNo() {
        return _sequenceNumber;
    }
};

class QUICPacket {
	QUICHeader *_header;
	string _message;

public:
	QUICPacket(int seq, string msg) {
		_header = new QUICHeader(seq);
		_message = msg;
	}

    string serialString() {
        return (to_string(_header->getSeqNo())+"--"+_message);
    }
};