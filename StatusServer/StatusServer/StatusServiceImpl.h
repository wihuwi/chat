#pragma once
#include "message.grpc.pb.h"
#include <grpc/grpc.h>

using message::StatusService;
using message::GetChatServerReq;
using message::GetChatServerRsp;
using grpc::Status;
using grpc::ServerContext;
using grpc::Server;
using grpc::ServerBuilder;

struct ChatServer {
	ChatServer(const std::string h, const std::string p):host(h),port(p){}
	std::string host;
	std::string port;
};

class StatusServiceImpl final: public StatusService::Service
{
public:
	StatusServiceImpl();
	Status GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply) override;

	std::vector<ChatServer> _servers;
	int _server_index;
};

