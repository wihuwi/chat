#pragma once

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using message::VarifyService;
using message::GetVarifyReq;
using message::GetVarifyRsp;

class RPConPool {
public:
	RPConPool(const int poolSize, const std::string host, const std::string port);
	RPConPool();
	void Close();
	std::unique_ptr<VarifyService::Stub> GetConnection();
	void ReturnConnection(std::unique_ptr<VarifyService::Stub>);
private:
	std::atomic<bool> _b_stop;
	std::size_t _poolSize;
	std::string _host;
	std::string _port;
	std::mutex _mutex;
	std::condition_variable _cond;
	std::queue<std::unique_ptr<VarifyService::Stub>> _connections;
	std::thread _threads;
};

class VerifyGrpcClient:public Singleton<VerifyGrpcClient>
{
	friend Singleton<VerifyGrpcClient>;
public:
	~VerifyGrpcClient() = default;
	GetVarifyRsp GetVarifyCode(const std::string email);
private:
	VerifyGrpcClient();

	std::unique_ptr<RPConPool> _pool;
};

