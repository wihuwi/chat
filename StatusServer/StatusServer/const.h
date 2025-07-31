#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <mutex>
#include <functional>
#include <map>
#include <unordered_map>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <assert.h>

//#include "Singleton.h"
//#include "hiredis.h"

using namespace boost;

enum ErrorCodes {
	Success = 0,
	Error_Json = 1001,  //Json��������
	RPCFailed = 1002,  //RPC�������
	VarifyExpired = 1003, //��֤�����
	VarifyCodeErr = 1004, //��֤�����
	UserExist = 1005,       //�û��Ѿ�����
	PasswdErr = 1006,    //�������
	EmailNotMatch = 1007,  //���䲻ƥ��
	PasswdUpFailed = 1008,  //��������ʧ��
	PasswdInvalid = 1009,   //�������ʧ��
	TokenInvalid = 1010,   //TokenʧЧ
	UidInvalid = 1011  //uid��Ч
};

#define CODEPREFIX "code_"

class Defer {
public:
	Defer(std::function<void()> func) {
		_func = func;
	}
	~Defer() {
		_func();
	}
private:
	std::function<void()> _func;
};