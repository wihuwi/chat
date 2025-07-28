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
	Error_json = 1001,
	RPCRrror = 1002,
	VarifyExpired = 1003,
	VarifyCodeErr = 1004,
	UserExist = 1005,
	PasswdErr = 1006,
	EmailNotMatch = 1007,
	PasswdUpFailed = 1008,
	PasswdInvalid = 1009,
	RPCGetFailed = 1010
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