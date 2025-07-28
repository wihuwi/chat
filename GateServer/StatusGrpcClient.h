#pragma once

#include "const.h"
#include "Singleton.h"
#include "message.grpc.pb.h"
#include "message.pb.h"

#include <grpcpp\create_channel.h>

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;
using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;

class StatusConPool {
public:
	StatusConPool(const std::string& host, const std::string& port, int poolSize) :
		_host(host), _port(port), _poolSize(poolSize), _b_stop(false) {
		for (int i = 0; i < poolSize; i++) {
			std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
			_connections.push(StatusService::NewStub(channel));
		}
	}

	std::unique_ptr<StatusService::Stub> GetConnection() {
		std::unique_lock<std::mutex> lock(_mutex);
		_cond.wait(lock, [this]() {
			if (_b_stop) {
				return true;
			}
			return !_connections.empty();
			});
		
		if (_b_stop) {
			return nullptr;
		}

		auto con = std::move(_connections.front());
		_connections.pop();
		return con;
	}

	void ReturnConnection(std::unique_ptr<StatusService::Stub> con) {
		std::lock_guard<std::mutex> lock(_mutex);
		if (_b_stop) {
			return;
		}
		_connections.push(std::move(con));
		_cond.notify_one();
	}

	void Close() {
		_b_stop = true;
		_cond.notify_all();
	}

	~StatusConPool() {
		std::lock_guard<std::mutex> lock(_mutex);
		Close();
		while (!_connections.empty()) {
			_connections.pop();
		}
	}

private:
	std::string _host;
	std::string _port;
	std::atomic<bool> _b_stop;
	int _poolSize;
	std::queue<std::unique_ptr<StatusService::Stub>> _connections;
	std::mutex _mutex;
	std::condition_variable _cond;
};

class StatusGrpcClient:public Singleton<StatusGrpcClient>
{
	friend Singleton<StatusGrpcClient>;
public:
	~StatusGrpcClient();
	GetChatServerRsp GetChatServer(int uid);

private:
	StatusGrpcClient();
	
	std::unique_ptr<StatusConPool> _pool;
};

