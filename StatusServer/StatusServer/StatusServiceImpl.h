#pragma once
#include "message.grpc.pb.h"
#include <grpc/grpc.h>
#include <unordered_map>
#include <mutex>

using message::StatusService;
using message::GetChatServerReq;
using message::GetChatServerRsp;
using grpc::Status;
using grpc::ServerContext;
using grpc::Server;
using grpc::ServerBuilder;

struct ChatServer {
	std::string host;
	std::string port;
	std::string name;
	int con_count;
};

class StatusServiceImpl final: public StatusService::Service
{
public:
	StatusServiceImpl();
	void InsertToken(int uid, std::string token);
	ChatServer getChatServer();
	Status GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply) override;
	Status Login(ServerContext* context, const message::LoginReq* request, message::LoginRsp* reply) override;

	std::unordered_map<std::string, ChatServer> _servers;
	std::mutex _server_mutex;
	int _server_index;
	std::unordered_map<int, std::string> _tokens;
	std::mutex _token_mutex;
};

