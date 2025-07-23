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

class VerifyGrpcClient:public Singleton<VerifyGrpcClient>
{
	friend Singleton<VerifyGrpcClient>;
public:
	~VerifyGrpcClient() = default;
	GetVarifyRsp GetVarifyCode(const std::string email);
private:
	VerifyGrpcClient();

	std::unique_ptr<VarifyService::Stub> _stub;
};

