#pragma once

#include <map>
#include <memory>
#include <mutex>
#include "Singleton.h"
#include "CSession.h"

class UserMgr: public Singleton<UserMgr>
{
	friend Singleton<UserMgr>;
public:
	~UserMgr();
	std::shared_ptr<CSession> GetSession(int uid);
	void SetUserSession(int uid, std::shared_ptr<CSession> session);
	void RmvUserSession(int uid);

private:
	UserMgr();

	std::map<int, std::shared_ptr<CSession>> _uid_to_session;
	std::mutex _session_mtx;
};

