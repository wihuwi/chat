#pragma once
#include "const.h"
#include "Singleton.h"

class RedisConPool {
public:
	RedisConPool(int size, const char* host, int port, const char* pwd);
	~RedisConPool();
	redisContext* GetConncetion();
	void ReturnConnection(redisContext*);
	void Close();

private:
	int _poolSize;
	std::atomic<bool> _b_stop;
	std::mutex _mutex;
	std::condition_variable _cond;
	std::queue<redisContext*> _connections;
	const char* _host;
	int _port;
};

class RedisMgr: public Singleton<RedisMgr>
{
	friend Singleton<RedisMgr>;
public:
	~RedisMgr();
	bool Get(const std::string& key, std::string& value);
	bool Set(const std::string& key, const std::string& value);
	bool LPop(const std::string& key, std::string& value);
	bool LPush(const std::string& key, const std::string& value);
	bool RPop(const std::string& key, std::string& value);
	bool RPush(const std::string& key, const std::string& value);
	bool HSet(const std::string& key, const std::string& hkey, const std::string& value);
	bool HSet(const char* key, const char* hkey, const char* value, size_t hvaluelen);
	std::string HGet(const std::string& key, const std::string& hkey);
	bool Del(const std::string& key);
	bool ExistsKey(const std::string& key);
	void Close();
private:
	RedisMgr();

	std::unique_ptr<RedisConPool> _redis_pool;
};

