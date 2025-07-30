#pragma once
#include <string>
#include <boost/asio.hpp>
#include "const.h"

class LogicSystem;

class MsgNode{
public:
	MsgNode(short maxlen) :_total_len(maxlen), _cur_len(0) {
		_data = new char[maxlen + 1];
		_data[_total_len] = '\0';
	}

	~MsgNode() {
		delete[] _data;
	}

	void Clear() {
		memset(_data, '\0', _total_len);
		_cur_len = 0;
	}

	short _total_len;
	short _cur_len;
	char* _data;
};

class RecvNode :public MsgNode {
	friend class LogicSystem;
public:
	RecvNode(short max_len, short msg_id);
private:
	short _msg_id;
};

class SendNode :public MsgNode {
	friend class LogicSystem;
public:
	SendNode(const char* msg, short max_len, short msg_id);
	SendNode(short max_len, short msg_id);
private:
	short _msg_id;
};