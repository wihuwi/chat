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
	auto& cfg = ConfigMgr::GetInstance();
	ChatServer server;
	server.port = cfg["ChatServer1"]["Port"];
	server.host = cfg["ChatServer1"]["Host"];
	server.con_count = 0;
	server.name = cfg["ChatServer1"]["Name"];
	_servers[server.name] = server;

	server.port = cfg["ChatServer2"]["Port"];
	server.host = cfg["ChatServer2"]["Host"];
	server.name = cfg["ChatServer2"]["Name"];
	server.con_count = 0;
	_servers[server.name] = server;
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply) {
	std::cout << "StatusServer receive" << std::endl;
	auto server = getChatServer();
	reply->set_host(server.host);
	reply->set_port(server.port);
	reply->set_error(ErrorCodes::Success);
	reply->set_token(GengerateUniqueCode());
	reply->set_error(ErrorCodes::Success);
	InsertToken(request->uid(), reply->token());
	return Status::OK;
}

Status StatusServiceImpl::Login(ServerContext* context, const message::LoginReq* request, message::LoginRsp* reply) {
	auto uid = request->uid();
	auto token = request->token();
	std::lock_guard<std::mutex> lock(_token_mutex);
	auto it = _tokens.find(uid);
	if (it == _tokens.end()) {
		reply->set_error(ErrorCodes::UidInvalid);
		return Status::OK;
	}
	if (it->second != token) {
		reply->set_error(ErrorCodes::TokenInvalid);
		return Status::OK;
	}

	reply->set_error(ErrorCodes::Success);
	reply->set_uid(uid);
	reply->set_token(token);
	return Status::OK;
}

void StatusServiceImpl::InsertToken(int uid, std::string token) {
	std::lock_guard<std::mutex> lock(_token_mutex);
	_tokens[uid] = token;
}

ChatServer StatusServiceImpl::getChatServer() {
	std::lock_guard<std::mutex> guard(_server_mutex);
	auto minServer = _servers.begin()->second;
	// 使用范围基于for循环
	for (const auto& server : _servers) {
		if (server.second.con_count < minServer.con_count) {
			minServer = server.second;
		}
	}

	return minServer;
}