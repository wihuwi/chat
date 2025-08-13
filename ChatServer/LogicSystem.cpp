#include "LogicSystem.h"
#include "StatusGrpcClient.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"
#include "UserMgr.h"

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
	auto uid = src_root["uid"].asInt();
	auto token = src_root["token"].asString();
	std::cout << "user login uid is  " << uid << " user token  is "
		<< token << std::endl;

	Json::Value rtvalue;
	Defer defer([this, &rtvalue, &session]() {
		std::string json_str = rtvalue.toStyledString();
		session->Send(json_str, MSG_IDS::MSG_CHAT_LOGIN_RSP);
		});
	
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";
	bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
	if (!success) {
		rtvalue["error"] = ErrorCodes::UidInvalid;
		return;
	}
	if (token_value != token) {
		rtvalue["error"] = ErrorCodes::TokenInvalid;
		return;
	}

	std::string base_key = USER_BASE_INFO + uid;
	std::shared_ptr<UserInfo> user_info = std::make_shared<UserInfo>();
	bool b_base = GetBaseInfo(base_key, uid, user_info);
	if (!b_base) {
		rtvalue["error"] = ErrorCodes::UidInvalid;
		return;
	}

	rtvalue["error"] = ErrorCodes::Success;
	rtvalue["uid"] = uid;
	rtvalue["pwd"] = user_info->pwd;
	rtvalue["name"] = user_info->name;
	rtvalue["email"] = user_info->email;
	rtvalue["nick"] = user_info->nick;
	rtvalue["desc"] = user_info->desc;
	rtvalue["sex"] = user_info->sex;
	rtvalue["icon"] = user_info->icon;
	
	//传输数据

	std::string servername = ConfigMgr::Inst().GetValue("SelfServer", "Name");
	std::string redis_ret = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, servername);
	//服务器人数++
	int count = 0;
	if (!redis_ret.empty()) {
		count = std::stoi(redis_ret);
	}
	count++;
	RedisMgr::GetInstance()->HSet(LOGIN_COUNT, servername, std::to_string(count));

	//记录用户登录服务器ip
	std::string ip_key = USERIPPREFIX + uid_str;
	RedisMgr::GetInstance()->Set(ip_key, servername);

	//方便之后的踢人
	UserMgr::GetInstance()->SetUserSession(uid, session);
}

bool LogicSystem::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo)
{
	std::string info_str;
	if (RedisMgr::GetInstance()->Get(base_key, info_str)) {
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str, root);
		userinfo->uid = root["uid"].asInt();
		userinfo->name = root["name"].asString();
		userinfo->pwd = root["pwd"].asString();
		userinfo->email = root["email"].asString();
		userinfo->nick = root["nick"].asString();
		userinfo->desc = root["desc"].asString();
		userinfo->sex = root["sex"].asInt();
		userinfo->icon = root["icon"].asString();
		std::cout << "user login uid is  " << userinfo->uid << " name  is "
			<< userinfo->name << " pwd is " << userinfo->pwd << " email is " << userinfo->email << std::endl;
	}
	else {
		std::shared_ptr<UserInfo> user_info = nullptr;
		user_info = MysqlMgr::GetInstance()->GetUser(uid);
		if (user_info == nullptr) {
			return false;
		}

		userinfo = user_info;

		Json::Value redis_root;
		redis_root["uid"] = uid;
		redis_root["pwd"] = userinfo->pwd;
		redis_root["name"] = userinfo->name;
		redis_root["email"] = userinfo->email;
		redis_root["nick"] = userinfo->nick;
		redis_root["desc"] = userinfo->desc;
		redis_root["sex"] = userinfo->sex;
		redis_root["icon"] = userinfo->icon;
		RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());
	}
	return true;
}
