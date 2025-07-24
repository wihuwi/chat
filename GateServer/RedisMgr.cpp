#include "RedisMgr.h"

RedisConPool::RedisConPool(int size, const char* host, int port, const char* pwd) :
	_poolSize(0), _host(host), _port(port) {
	for (int i = 0; i < size; i++) {
		redisContext* con = redisConnect(host, port);
		if (con == NULL || con->err != 0) {
			std::cout << "redis connect failed" << std::endl;
			if (con != nullptr) {
				redisFree(con);
				//Ïú»Ù
			}
			return;
		}
		redisReply* reply = (redisReply*)redisCommand(con, "AUTH %s", pwd);

		if (reply->type == REDIS_REPLY_ERROR) {
			std::cout << "redis auth failed" << std::endl;
			freeReplyObject(reply);
			return;
		}
		freeReplyObject(reply);
		_connections.push(con);
		_poolSize++;
	}
}

RedisConPool::~RedisConPool() {
	std::lock_guard<std::mutex> lock(_mutex);
	while (!_connections.empty()) {
		_connections.pop();
	}
}

redisContext* RedisConPool::GetConncetion() {
	std::unique_lock<std::mutex> lock(_mutex);
	_cond.wait(lock, [this] {
		if (_b_stop) {
			return true;
		}
		return !_connections.empty();
		});
	if (_b_stop) {
		return nullptr;
	}
	auto con = _connections.front();
	_connections.pop();
	return con;
}

void RedisConPool::ReturnConnection(redisContext* con) {
	std::lock_guard<std::mutex> lock(_mutex);
	if (_b_stop) {
		return;
	}
	_connections.push(con);
	_cond.notify_one();
}

void RedisConPool::Close() {
	_b_stop = true;
	_cond.notify_all();
}

RedisMgr::RedisMgr() {
	auto host = ConfigMgr::GetInstance()["Redis"]["Host"];
	auto port = ConfigMgr::GetInstance()["Redis"]["Port"];
	auto passwd = ConfigMgr::GetInstance()["Redis"]["Passwd"];
	_redis_pool.reset(new RedisConPool(5, host.c_str(), atoi(port.c_str()), passwd.c_str()));
}

RedisMgr::~RedisMgr() {
	Close();
}

bool RedisMgr::Get(const std::string& key, std::string& value) {
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	auto reply = (redisReply*)redisCommand(connect, "GET %s", key.c_str());
	if (reply == NULL || reply->type != REDIS_REPLY_STRING) {
		std::cout << "Execut Get [key" << key << "] failed" << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}

	value = reply->str;
	freeReplyObject(reply);
	std::cout << "Execut Get [" << key << "] success" << std::endl;
	_redis_pool->ReturnConnection(connect);
	return true;
}

bool RedisMgr::Set(const std::string& key, const std::string& value) {
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	auto reply = (redisReply*)redisCommand(connect, "Set %s %s", key.c_str(), value.c_str());
	if (reply == NULL) {
		std::cout << "Execut Set [key" << key << " " << value << "] failed" << std::endl;
		freeReplyObject(reply);
		_redis_pool->ReturnConnection(connect);
		return false;
	}

	if (!(reply->type == REDIS_REPLY_STATUS && (strcmp(reply->str, "OK") == 0 || strcmp(reply->str, "ok") == 0))) {
		std::cout << "Execut Set [key" << key << " " << value << "] failed" << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}

	freeReplyObject(reply);
	std::cout << "Execut Set [key" << key << " " << value << "] success" << std::endl;
	_redis_pool->ReturnConnection(connect);

	return true;
}

bool RedisMgr::LPush(const std::string& key, const std::string& value) {
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	auto reply = (redisReply*)redisCommand(connect, "LPush %s %s", key.c_str(), value.c_str());
	if (reply == NULL) {
		std::cout << "Execut LPush [key" << key << " " << value << "] failed" << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}
	if (reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0) {
		std::cout << "Execut LPush [key" << key << " " << value << "] failed" << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}

	freeReplyObject(reply);
	_redis_pool->ReturnConnection(connect);
	std::cout << "Execut LPush [key" << key << " " << value << "] success" << std::endl;
	return true;
}

bool RedisMgr::LPop(const std::string& key, std::string& value) {
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	auto reply = (redisReply*)redisCommand(connect, "LPop %s", key.c_str());
	if (reply == NULL || reply->type == REDIS_REPLY_NIL) {
		std::cout << "Execut LPop [key" << key << " " << value << "] failed" << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}

	value = reply->str;
	_redis_pool->ReturnConnection(connect);
	freeReplyObject(reply);
	std::cout << "Execut LPop [key" << key << " " << value << "] success" << std::endl;
	return true;
}

bool RedisMgr::RPush(const std::string& key, const std::string& value) {
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	auto reply = (redisReply*)redisCommand(connect, "RPush %s %s", key.c_str(), value.c_str());
	if (reply == NULL) {
		std::cout << "Execut RPush [key" << key << " " << value << "] failed" << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}
	if (reply->type != REDIS_REPLY_INTEGER || reply->integer <= 0) {
		std::cout << "Execut RPush [key" << key << " " << value << "] failed" << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}

	freeReplyObject(reply);
	_redis_pool->ReturnConnection(connect);
	std::cout << "Execut RPush [key" << key << " " << value << "] success" << std::endl;
	return true;
}

bool RedisMgr::RPop(const std::string& key, std::string& value) {
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	auto reply = (redisReply*)redisCommand(connect, "RPop %s", key.c_str());
	if (reply == NULL || reply->type == REDIS_REPLY_NIL) {
		_redis_pool->ReturnConnection(connect);
		std::cout << "Execut RPop [key" << key << " " << value << "] failed" << std::endl;
		freeReplyObject(reply);
		return false;
	}

	value = reply->str;
	_redis_pool->ReturnConnection(connect);
	freeReplyObject(reply);
	std::cout << "Execut RPop [key" << key << " " << value << "] success" << std::endl;
	return true;
}

bool RedisMgr::HSet(const std::string& key, const std::string& hkey, const std::string& value) {
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	auto reply = (redisReply*)redisCommand(connect, "HSET %s %s %s", key.c_str(), hkey.c_str(), value.c_str());
	if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << value << " ] failure ! " << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}
	std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << value << " ] success ! " << std::endl;
	_redis_pool->ReturnConnection(connect);
	freeReplyObject(reply);
	return true;
}

bool RedisMgr::HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen)
{
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	const char* argv[4];
	size_t argvlen[4];
	argv[0] = "HSET";
	argvlen[0] = 4;
	argv[1] = key;
	argvlen[1] = strlen(key);
	argv[2] = hkey;
	argvlen[2] = strlen(hkey);
	argv[3] = hvalue;
	argvlen[3] = hvaluelen;
	auto reply = (redisReply*)redisCommandArgv(connect, 4, argv, argvlen);
	if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] failure ! " << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}
	std::cout << "Execut command [ HSet " << key << "  " << hkey << "  " << hvalue << " ] success ! " << std::endl;
	_redis_pool->ReturnConnection(connect);
	freeReplyObject(reply);
	return true;
}

std::string RedisMgr::HGet(const std::string& key, const std::string& hkey)
{
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return "";
	}
	const char* argv[3];
	size_t argvlen[3];
	argv[0] = "HGET";
	argvlen[0] = 4;
	argv[1] = key.c_str();
	argvlen[1] = key.length();
	argv[2] = hkey.c_str();
	argvlen[2] = hkey.length();
	auto reply = (redisReply*)redisCommandArgv(connect, 3, argv, argvlen);
	if (reply == nullptr || reply->type == REDIS_REPLY_NIL) {
		freeReplyObject(reply);
		std::cout << "Execut command [ HGet " << key << " " << hkey << "  ] failure ! " << std::endl;
		_redis_pool->ReturnConnection(connect);
		return "";
	}
	std::string value = reply->str;
	_redis_pool->ReturnConnection(connect);
	freeReplyObject(reply);
	std::cout << "Execut command [ HGet " << key << " " << hkey << " ] success ! " << std::endl;
	return value;
}

bool RedisMgr::Del(const std::string& key)
{
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	auto reply = (redisReply*)redisCommand(connect, "DEL %s", key.c_str());
	if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER) {
		std::cout << "Execut command [ Del " << key << " ] failure ! " << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}
	std::cout << "Execut command [ Del " << key << " ] success ! " << std::endl;
	_redis_pool->ReturnConnection(connect);
	freeReplyObject(reply);
	return true;
}

bool RedisMgr::ExistsKey(const std::string& key)
{
	redisContext* connect = _redis_pool->GetConncetion();
	if (connect == nullptr) {
		return false;
	}
	auto reply = (redisReply*)redisCommand(connect, "exists %s", key.c_str());
	if (reply == nullptr || reply->type != REDIS_REPLY_INTEGER || reply->integer == 0) {
		std::cout << "Not Found [ Key " << key << " ]  ! " << std::endl;
		_redis_pool->ReturnConnection(connect);
		freeReplyObject(reply);
		return false;
	}
	std::cout << " Found [ Key " << key << " ] exists ! " << std::endl;
	_redis_pool->ReturnConnection(connect);
	freeReplyObject(reply);
	return true;
}

void RedisMgr::Close() {
	_redis_pool->Close();
}