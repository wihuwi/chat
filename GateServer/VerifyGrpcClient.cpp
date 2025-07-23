#include "VerifyGrpcClient.h"

GetVarifyRsp VerifyGrpcClient::GetVarifyCode(const std::string email) {
	GetVarifyReq request;
	GetVarifyRsp reply;
	ClientContext context;
	request.set_email(email);

	Status status = _stub->GetVarifyCode(&context, request, &reply);
	if (status.ok()) {
		return reply;
	}
	else {
		reply.set_error(ErrorCodes::RPCRrror);
		return reply;
	}
}

VerifyGrpcClient::VerifyGrpcClient() {
	std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());
	_stub = VarifyService::NewStub(channel);
}