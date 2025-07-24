#include "VerifyGrpcClient.h"
#include "config.h"

RPConPool::RPConPool(const int poolSize, const std::string host, const std::string port) :
_poolSize(poolSize), _host(host), _port(port){
	for (int i = 0; i < poolSize; i++) {
		//std::string a = _host + ":" + _port;
		std::string a = "\"" + _host + ":" + _port + "\"";

		std::cout << a << std::endl;
		std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
		_connections.push(VarifyService::NewStub(channel));
	}
}

RPConPool::RPConPool() {
	std::lock_guard<std::mutex> lock(_mutex);
	Close();
	while (!_connections.empty()) {
		_connections.pop();
	}
}

void RPConPool::Close() {
	_b_stop = true;
	_cond.notify_all();
}

std::unique_ptr<VarifyService::Stub> RPConPool::GetConnection() {
	std::unique_lock<std::mutex> lock(_mutex);
	//while (_connections.empty()) {
	//	_cond.wait(lock);
	//	if (_b_stop) {
	//		return nullptr;
	//	}
	//}
	_cond.wait(lock, [this]() {
		if (_b_stop) {
			return true;
		}
		return !_connections.empty();
		});

	if (_b_stop) {
		return nullptr;
	}

	std::unique_ptr<VarifyService::Stub> stub = std::move(_connections.front());
	_connections.pop();
	return stub;
}

void RPConPool::ReturnConnection(std::unique_ptr<VarifyService::Stub> stub) {
	std::lock_guard<std::mutex>lock(_mutex);
	if (_b_stop) {
		return;
	}
	_connections.push(std::move(stub));
	_cond.notify_all();
}

GetVarifyRsp VerifyGrpcClient::GetVarifyCode(const std::string email) {
	GetVarifyReq request;
	GetVarifyRsp reply;
	ClientContext context;
	request.set_email(email);

	auto stub = _pool->GetConnection();
	std::cout << stub << std::endl;
	Status status = stub->GetVarifyCode(&context, request, &reply);
	if (status.ok()) {
		_pool->ReturnConnection(std::move(stub));
		return reply;
	}
	else {
		std::cout << "RPC failed.\n"
			<< "Error code: " << status.error_code() << "\n"
			<< "Error message: " << status.error_message() << "\n"
			<< "Error details: " << status.error_details() << std::endl;

		reply.set_error(ErrorCodes::RPCRrror);
		_pool->ReturnConnection(std::move(stub));
		return reply;
	}
}

VerifyGrpcClient::VerifyGrpcClient() {
	std::string host = ConfigMgr::GetInstance()["VarifyServer"]["Host"];
	std::string port = ConfigMgr::GetInstance()["VarifyServer"]["Port"];
	_pool.reset(new RPConPool(5, host, port));
}