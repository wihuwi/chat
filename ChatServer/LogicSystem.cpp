#include "LogicSystem.h"
#include "StatusGrpcClient.h"
#include "ChatGrpcClient.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"
#include "UserMgr.h"
#include "ConfigMgr.h"

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
	_fun_callbacks[MSG_IDS::ID_SEARCH_USER_REQ] = std::bind(&LogicSystem::SearchInfo, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	_fun_callbacks[MSG_IDS::ID_ADD_FRIEND_REQ] = std::bind(&LogicSystem::AddFriendApply, this,
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
	
	//验证信息
	std::vector<std::shared_ptr<ApplyInfo>> apply_list;
	auto b_apply = GetFriendApplyInfo(uid, apply_list);
	if (b_apply) {
		for (auto& apply : apply_list) {
			Json::Value obj;
			obj["name"] = apply->_name;
			obj["uid"] = apply->_uid;
			obj["icon"] = apply->_icon;
			obj["nick"] = apply->_nick;
			obj["sex"] = apply->_sex;
			obj["desc"] = apply->_desc;
			obj["status"] = apply->_status;
			rtvalue["apply_list"].append(obj);
		}
	}

	//好友信息

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

void LogicSystem::SearchInfo(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data) {
	//传进来的uid 可能是uid 也可能是名字
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	auto uid_str = root["uid"].asString();
	std::cout << "user SearchInfo uid is  " << uid_str << std::endl;

	Json::Value  rtvalue;

	Defer defer([this, &rtvalue, session]() {
		std::string return_str = rtvalue.toStyledString();
		session->Send(return_str, ID_SEARCH_USER_RSP);
		});

	bool b_digit = isPureDigit(uid_str);
	if (b_digit) {
		GetUserByUid(uid_str, rtvalue);
	}
	else {
		GetUserByName(uid_str, rtvalue);
	}
}

void LogicSystem::AddFriendApply(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data)
{
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	auto uid = root["uid"].asInt();
	auto applyname = root["applyname"].asString();
	auto bakname = root["bakname"].asString();
	auto touid = root["touid"].asInt();
	std::cout << "user login uid is  " << uid << " applyname  is "
		<< applyname << " bakname is " << bakname << " touid is " << touid << std::endl;

	Json::Value  rtvalue;
	rtvalue["error"] = ErrorCodes::Success;
	Defer defer([this, &rtvalue, session]() {
		std::string return_str = rtvalue.toStyledString();
		session->Send(return_str, ID_ADD_FRIEND_RSP);
		//这个 send 是写给发送请求的client的，告诉他发送成功与否
		//之后的 send 是写给被申请人，告诉他有人申请好友
		});

	//更新数据库
	MysqlMgr::GetInstance()->AddFriendApply(uid, touid);

	auto to_str = std::to_string(touid);
	auto to_ip_key = USERIPPREFIX + to_str;
	std::string to_ip_value = "";
	bool b_ip = RedisMgr::GetInstance()->Get(to_ip_key, to_ip_value);
	if (!b_ip) {
		//这里感觉需要设置一个error
		return;
	}

	auto& cfg = ConfigMgr::Inst();
	auto self_name = cfg["SelfServer"]["Name"];


	std::string base_key = USER_BASE_INFO + std::to_string(uid);
	auto apply_info = std::make_shared<UserInfo>();
	bool b_info = GetBaseInfo(base_key, uid, apply_info);

	//用户存在当前服务器
	if (to_ip_value == self_name) {
		auto session = UserMgr::GetInstance()->GetSession(touid);
		if (session) {
			//获取被申请人的 会话session，也就是看他在不在线
			Json::Value  notify;
			notify["error"] = ErrorCodes::Success;
			notify["applyuid"] = uid;
			notify["name"] = applyname;
			notify["desc"] = "";
			if (b_info) {
				notify["icon"] = apply_info->icon;
				notify["sex"] = apply_info->sex;
				notify["nick"] = apply_info->nick;
			}
			std::string return_str = notify.toStyledString();
			session->Send(return_str, ID_NOTIFY_ADD_FRIEND_REQ);
			//向被申请人发送申请信息
		}

		return;
	}

	//用户存在不同服务器
	AddFriendReq add_req;
	add_req.set_applyuid(uid);
	add_req.set_touid(touid);
	add_req.set_name(applyname);
	add_req.set_desc("");
	if (b_info) {
		add_req.set_icon(apply_info->icon);
		add_req.set_sex(apply_info->sex);
		add_req.set_nick(apply_info->nick);
	}

	//向目标服务器发送
	ChatGrpcClient::GetInstance()->NotifyAddFriend(to_ip_value, add_req);
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

bool LogicSystem::isPureDigit(std::string str) {
	for (auto s : str) {
		if (!std::isdigit(s)) {
			return false;
		}
	}
	return true;
}

void LogicSystem::GetUserByUid(std::string uid_str, Json::Value& rtvalue) {
	std::string base_key = USER_BASE_INFO + uid_str;
	std::string info_str = "";
	bool b_base = RedisMgr::GetInstance()->Get(base_key, info_str);
	if (b_base) {
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str, root);
		auto uid = root["uid"].asInt();
		auto name = root["name"].asString();
		auto pwd = root["pwd"].asString();
		auto email = root["email"].asString();
		auto nick = root["nick"].asString();
		auto desc = root["desc"].asString();
		auto sex = root["sex"].asInt();
		auto icon = root["icon"].asString();
		std::cout << "user  uid is  " << uid << " name  is "
			<< name << " pwd is " << pwd << " email is " << email << " icon is " << icon << std::endl;

		rtvalue["uid"] = uid;
		rtvalue["pwd"] = pwd;
		rtvalue["name"] = name;
		rtvalue["email"] = email;
		rtvalue["nick"] = nick;
		rtvalue["desc"] = desc;
		rtvalue["sex"] = sex;
		rtvalue["icon"] = icon;
		rtvalue["error"] = ErrorCodes::Success;
		return;
	}

	auto uid = std::stoi(uid_str);
	std::shared_ptr<UserInfo> user_info = nullptr;
	user_info = MysqlMgr::GetInstance()->GetUser(uid);
	if (user_info == nullptr) {
		rtvalue["error"] = ErrorCodes::UidInvalid;
		return;
	}

	Json::Value redis_root;
	redis_root["uid"] = user_info->uid;
	redis_root["pwd"] = user_info->pwd;
	redis_root["name"] = user_info->name;
	redis_root["email"] = user_info->email;
	redis_root["nick"] = user_info->nick;
	redis_root["desc"] = user_info->desc;
	redis_root["sex"] = user_info->sex;
	redis_root["icon"] = user_info->icon;

	RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());

	rtvalue["uid"] = user_info->uid;
	rtvalue["pwd"] = user_info->pwd;
	rtvalue["name"] = user_info->name;
	rtvalue["email"] = user_info->email;
	rtvalue["nick"] = user_info->nick;
	rtvalue["desc"] = user_info->desc;
	rtvalue["sex"] = user_info->sex;
	rtvalue["icon"] = user_info->icon;
	rtvalue["error"] = ErrorCodes::Success;
}

void LogicSystem::GetUserByName(std::string name, Json::Value& rtvalue)
{
	rtvalue["error"] = ErrorCodes::Success;

	std::string base_key = NAME_INFO + name;

	std::string info_str = "";
	bool b_base = RedisMgr::GetInstance()->Get(base_key, info_str);
	if (b_base) {
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str, root);
		auto uid = root["uid"].asInt();
		auto name = root["name"].asString();
		auto pwd = root["pwd"].asString();
		auto email = root["email"].asString();
		auto nick = root["nick"].asString();
		auto desc = root["desc"].asString();
		auto sex = root["sex"].asInt();
		std::cout << "user  uid is  " << uid << " name  is "
			<< name << " pwd is " << pwd << " email is " << email << std::endl;

		rtvalue["uid"] = uid;
		rtvalue["pwd"] = pwd;
		rtvalue["name"] = name;
		rtvalue["email"] = email;
		rtvalue["nick"] = nick;
		rtvalue["desc"] = desc;
		rtvalue["sex"] = sex;
		rtvalue["error"] = ErrorCodes::Success;
		return;
	}

	std::shared_ptr<UserInfo> user_info = nullptr;
	user_info = MysqlMgr::GetInstance()->GetUser(name);
	if (user_info == nullptr) {
		rtvalue["error"] = ErrorCodes::UidInvalid;
		return;
	}

	Json::Value redis_root;
	redis_root["uid"] = user_info->uid;
	redis_root["pwd"] = user_info->pwd;
	redis_root["name"] = user_info->name;
	redis_root["email"] = user_info->email;
	redis_root["nick"] = user_info->nick;
	redis_root["desc"] = user_info->desc;
	redis_root["sex"] = user_info->sex;

	RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());

	rtvalue["uid"] = user_info->uid;
	rtvalue["pwd"] = user_info->pwd;
	rtvalue["name"] = user_info->name;
	rtvalue["email"] = user_info->email;
	rtvalue["nick"] = user_info->nick;
	rtvalue["desc"] = user_info->desc;
	rtvalue["error"] = ErrorCodes::Success;
	rtvalue["sex"] = user_info->sex;
}

bool LogicSystem::GetFriendApplyInfo(int uid, std::vector<std::shared_ptr<ApplyInfo>>& list)
{
	return MysqlMgr::GetInstance()->GetApplyList(uid, list, 0, 10);
}
