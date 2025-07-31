#pragma once
#include <mutex>
#include <memory>
#include <queue>
#include <map>
#include <functional>
#include <json/json.h>

#include "Singleton.h"
#include "CSession.h"

typedef std::function<void(std::shared_ptr<CSession>, const short& msg_id, const std::string& msg_data)> FunCallBack;

class LogicSystem:public Singleton<LogicSystem>
{
	friend Singleton<LogicSystem>;
public:
	~LogicSystem();
	void PostMsgToQue(std::shared_ptr<LogicNode>);

private:
	LogicSystem();
	void DealMsg();
	void RegisterCallBacks();
	void HellowordCallBack(std::shared_ptr<CSession>, const short& msg_id, const std::string& msg_data);
	void LoginHandler(std::shared_ptr<CSession> session, const short& msg_id, const std::string& msg_data);

	std::queue<std::shared_ptr<LogicNode>> _msg_que;
	std::thread _work_thread;
	std::condition_variable _cond;
	std::mutex _mutex;
	bool _b_stop;
	std::map<short, FunCallBack> _fun_callbacks;
};

