#include "StatusGrpcClient.h"

StatusGrpcClient::StatusGrpcClient() {
	std::string host = ConfigMgr::GetInstance()["StatusServer"]["Host"];
	std::string port = ConfigMgr::GetInstance()["StatusServer"]["Port"];
	_pool = std::make_unique<StatusConPool>(host, port, 5);
}

StatusGrpcClient::~StatusGrpcClient() {

}

GetChatServerRsp StatusGrpcClient::GetChatServer(int uid) {
	GetChatServerRsp reply;
	ClientContext context;
	GetChatServerReq req;
	req.set_uid(uid);
	auto con = _pool->GetConnection();
	Defer defer([this, &con]() {
		_pool->ReturnConnection(std::move(con));
		});
	Status status = con->GetChatServer(&context, req, &reply);
	if (status.ok()) {
		return reply;
	}
	else {
		reply.set_error(ErrorCodes::RPCRrror);
		return reply;
	}
}

message::LoginRsp StatusGrpcClient::Login(int uid, std::string token) {
	ClientContext context;
	message::LoginReq request;
	message::LoginRsp reply;
	auto stub = _pool->GetConnection();
	Defer defer([this, &stub]() {
		_pool->ReturnConnection(std::move(stub));
		});
	Status status = stub->Login(&context, request, &reply);
	if (status.ok()) {
		return reply;
	}
	else {
		reply.set_error(ErrorCodes::RPCGetFailed);
		return reply;
	}
}
