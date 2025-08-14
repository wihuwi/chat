#pragma once

#include <grpcpp/grpcpp.h>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>
#include <atomic>
#include <unordered_map>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include "Singleton.h"
#include "ConfigMgr.h"
#include "Mysqldao.h"

using grpc::ClientContext;
using grpc::Channel;
using grpc::Status;

using message::AddFriendReq;
using message::AddFriendRsp;
using message::AuthFriendReq;
using message::AuthFriendRsp;
using message::TextChatMsgReq;
using message::TextChatMsgRsp;

using message::ChatService;

class ChatConPool {
public:
	ChatConPool(int pool_size, std::string host, std::string port) :_host(host),
		_port(port), _pool_size(pool_size), _b_stop(false) {
		for (int i = 0; i < pool_size; i++) {
			std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
			std::cout << channel << std::endl;
			_connections.push(ChatService::NewStub(channel));
		}
	}
	~ChatConPool() {
		std::lock_guard<std::mutex> lock(_mutex);
		while (!_connections.empty()) {
			_connections.pop();
		}
	}
	std::unique_ptr<ChatService::Stub> GetConnection() {
		std::unique_lock<std::mutex> lock(_mutex);
		_cond.wait(lock, [this]() {
			if (_b_stop) {
				return false;
			}
			return !_connections.empty();
			});

		if (_b_stop) {
			return nullptr;
		}
		std::unique_ptr<ChatService::Stub> con = std::move(_connections.front());
		_connections.pop();
		return std::move(con);
	}
	void ReturnConnection(std::unique_ptr<ChatService::Stub> stub) {
		std::lock_guard<std::mutex> lock(_mutex);
		if (_b_stop) {
			return;
		}
		_connections.push(std::move(stub));
		_cond.notify_one();
	}
	void Close() {
		_b_stop = true;
		_cond.notify_all();
	}

private:
	std::string _host;
	std::string _port;
	int _pool_size;
	std::queue<std::unique_ptr<ChatService::Stub>> _connections;
	std::mutex _mutex;
	std::condition_variable _cond;
	std::atomic<bool> _b_stop;
};

class ChatGrpcClient: public Singleton<ChatGrpcClient>
{
	friend Singleton<ChatGrpcClient>;
public:
	~ChatGrpcClient();
	AddFriendRsp NotifyAddFriend(std::string server_ip, const AddFriendReq& req);
	AuthFriendRsp NotifyAuthFriend(std::string server_ip, const AuthFriendReq& req);
	bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);
	TextChatMsgRsp NotifyTextChatMsg(std::string server_ip, const TextChatMsgReq& req, const Json::Value& rtvalue);

private:
	ChatGrpcClient();

	std::unordered_map<std::string, std::unique_ptr<ChatConPool>> _pools;
};

