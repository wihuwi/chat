#include "StatusServiceImpl.h"
#include "ConfigMgr.h"

#include <boost/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

std::string GengerateUniqueCode() {
	boost::uuids::uuid uid = boost::uuids::random_generator()();

	std::string uuid = to_string(uid);
	return uuid;
}

StatusServiceImpl::StatusServiceImpl():_server_index(0) {
	std::string host = ConfigMgr::GetInstance()["ChatServer1"]["Host"];
	std::string port = ConfigMgr::GetInstance()["ChatServer1"]["Port"];
	_servers.push_back(ChatServer(host, port));
	host = ConfigMgr::GetInstance()["ChatServer1"]["Host"];
	port = ConfigMgr::GetInstance()["ChatServer1"]["Port"];
	_servers.push_back(ChatServer(host, port));
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply) {
	std::cout << "StatusServer receive" << std::endl;
	_server_index = (++_server_index) % (_servers.size());
	auto server = _servers[_server_index];
	reply->set_host(server.host);
	reply->set_port(server.port);
	reply->set_token(GengerateUniqueCode());
	reply->set_error(ErrorCodes::Success);
	return Status::OK;
}