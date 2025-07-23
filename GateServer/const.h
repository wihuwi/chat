#pragma once

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
#include "ConfigMgr.h"

#include "Singleton.h"

using namespace boost;

enum ErrorCodes {
	Success = 1001,
	Error_json = 1002,
	RPCRrror = 1003
};

class ConfigMgr;
extern ConfigMgr gCfgMgr;