#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifyGrpcClient.h"
#include "message.grpc.pb.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"
#include <sstream>

LogicSystem::LogicSystem() {
	RegGet("/get_test", [](std::shared_ptr<HttpConnection> connect) {
		beast::ostream(connect->_response.body()) << "recv get_test" << std::endl;
		int i = 0;
		for (auto elem : connect->_get_params) {
			beast::ostream(connect->_response.body()) << "Param" << i << ": " << elem.first;
			beast::ostream(connect->_response.body()) << " Value" << i << ": " << elem.second << std::endl;
			i++;
		}
		});
	RegPost("/get_varifycode", [](std::shared_ptr<HttpConnection>connect) {
		std::string rawBuf = beast::buffers_to_string(connect->_request.body().data());
		std::cout << "recv: " << rawBuf << std::endl;
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool success = reader.parse(rawBuf, src_root);
		if (!success) {
			std::cout << "error json" << std::endl;
			root["error"] = Error_json;
			std::string jsonStr = root.toStyledString();
			beast::ostream(connect->_response.body()) << jsonStr;
			return true;
		}

		if (!src_root.isMember("email")) {
			std::cout << "error json" << std::endl;
			root["error"] = Error_json;
			std::string jsonStr = root.toStyledString();
			beast::ostream(connect->_response.body()) << jsonStr;
			return true;
		}

		std::string  email = src_root["email"].toStyledString();
		GetVarifyRsp reply = VerifyGrpcClient::GetInstance()->GetVarifyCode(email);
		std::cout << "email:" << email << std::endl;
		root["error"] = reply.error();
		root["email"] = src_root["email"];
		std::string jsonStr = root.toStyledString();
		beast::ostream(connect->_response.body()) << jsonStr;
		return true;
		});

	RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection) {
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout << "receive body is " << body_str << std::endl;
		connection->_response.set(beast::http::field::content_type, "text/json");
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}
		//先查找redis中email对应的验证码是否合理
		std::string  varify_code;
		bool b_get_varify = RedisMgr::GetInstance()->Get(CODEPREFIX + src_root["email"].asString(), varify_code);
		if (!b_get_varify) {
			std::cout << " get varify code expired" << std::endl;
			root["error"] = ErrorCodes::VarifyExpired;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}
		if (varify_code != src_root["varifycode"].asString()) {
			std::cout << " varify code error" << std::endl;
			root["error"] = ErrorCodes::VarifyCodeErr;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}
		//访问redis查找

		//查找数据库判断用户是否存在
		std::string name = src_root["user"].asString();
		std::string email = src_root["email"].asString();
		std::string passwd = src_root["passwd"].asString();
		int uid = MysqlMgr::GetInstance()->RegUser(name, email, passwd);
		if (uid == 0 || uid == -1) {
			std::cout << "email or name have existed" << std::endl;
			root["error"] = ErrorCodes::UserExist;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		root["error"] = 0;
		root["uid"] = uid;
		root["email"] = src_root["email"];
		root["user"] = src_root["user"].asString();
		root["passwd"] = src_root["passwd"].asString();
		root["confirm"] = src_root["confirm"].asString();
		root["varifycode"] = src_root["varifycode"].asString();
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << jsonstr;
		return true;
		});
}

void LogicSystem::RegGet(std::string url, HttpHandler handler) {
	_get_handlers.insert(std::pair<std::string, HttpHandler>(url, handler));
}

void LogicSystem::RegPost(std::string url, HttpHandler handler) {
	_post_handlers.insert(std::pair<std::string, HttpHandler>(url, handler));
}

bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> connection) {
	if (_get_handlers.find(path) == _get_handlers.end()) {
		return false;
	}

	_get_handlers[path](connection);
	return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> connection) {
	if (_post_handlers.find(path) == _post_handlers.end()) {
		return false;
	}

	_post_handlers[path](connection);
	return true;
}
