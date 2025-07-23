#include "LogicSystem.h"
#include "HttpConnection.h"
#include "VerifyGrpcClient.h"
#include "message.grpc.pb.h"

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
