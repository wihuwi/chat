#include "LogicSystem.h"
#include "StatusGrpcClient.h"

LogicSystem::LogicSystem() :_b_stop(false) {
	RegisterCallBacks();

	_work_thread = std::thread(&LogicSystem::DealMsg, this);
}

LogicSystem::~LogicSystem() {
	_b_stop = true;
	_cond.notify_one();
	_work_thread.join();
}

void LogicSystem::PostMsgToQue(std::shared_ptr<LogicNode> logic_node) {
	std::unique_lock<std::mutex> lock(_mutex);
	_msg_que.push(logic_node);
	if (_msg_que.size() == 1) {
		lock.unlock();
		_cond.notify_one();
	}
}

void LogicSystem::DealMsg() {
	while (1) {
		std::unique_lock<std::mutex> lock(_mutex);
		while (_msg_que.empty() && !_b_stop) {
			_cond.wait(lock);
		}

		if (_b_stop) {
			while (!_msg_que.empty()) {
				auto msgnode = _msg_que.front();
				std::cout << "recv_msg id  is " << msgnode->_node->_msg_id << std::endl;
				auto it = _fun_callbacks.find(msgnode->_node->_msg_id);
				if (it != _fun_callbacks.end()) {
					it->second(msgnode->_session, msgnode->_node->_msg_id, 
						std::string(msgnode->_node->_data, msgnode->_node->_total_len));
				}
				_msg_que.pop();
			}
			break;
		}

		auto msgnode = _msg_que.front();
		std::cout << "recv_msg id  is " << msgnode->_node->_msg_id << std::endl;
		auto it = _fun_callbacks.find(msgnode->_node->_msg_id);
		if (it != _fun_callbacks.end()) {
			it->second(msgnode->_session, msgnode->_node->_msg_id,
				std::string(msgnode->_node->_data, msgnode->_node->_total_len));
		}
		_msg_que.pop();
	}
}

void LogicSystem::RegisterCallBacks() {
	_fun_callbacks[MSG_IDS::MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void LogicSystem::HellowordCallBack(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	std::cout << "recevie msg id  is " << root["id"].asInt() << " msg data is "
		<< root["data"].asString() << std::endl;
	root["data"] = "server has received msg, msg data is " + root["data"].asString();
	std::string return_str = root.toStyledString();
	session->Send(return_str, root["id"].asInt());
}

void LogicSystem::LoginHandler(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data) {
	Json::Reader reader;
	Json::Value src_root;
	bool b_parse = reader.parse(msg_data, src_root);
	std::cout << "user login uid is  " << src_root["uid"].asInt() << " user token  is "
		<< src_root["token"].asString() << std::endl;
	message::LoginRsp reply = StatusGrpcClient::GetInstance()->Login(src_root["uid"].asInt(), src_root["token"].asString());
	Json::Value rt_root;
	rt_root["error"] = reply.error();
	rt_root["uid"] = reply.uid();
	rt_root["token"] = reply.token();
	std::string jsonStr = rt_root.toStyledString();
	session->Send(jsonStr, MSG_IDS::MSG_CHAT_LOGIN_RSP);
}