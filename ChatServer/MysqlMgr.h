#pragma once
#include "const.h"
#include "MysqlDao.h"
class MysqlMgr : public Singleton<MysqlMgr>
{
	friend class Singleton<MysqlMgr>;
public:
	~MysqlMgr();
	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
	bool CheckEmail(const std::string& name, const std::string& email);
	bool UpdatePwd(const std::string& name, const std::string& email);
	bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo);
	bool AddFriendApply(const int& from_uid, const int& to_uid);
	bool GetApplyList(int touid, std::vector<std::shared_ptr<ApplyInfo>>& list, int begin, int limit);
	bool AuthFriendApply(int from, int to);
	bool AddFriend(int from, int to, std::string backname);
	bool GetFriendList(int uid, std::vector<std::shared_ptr<UserInfo>>& list);
	std::shared_ptr<UserInfo> GetUser(int uid);
	std::shared_ptr<UserInfo> GetUser(std::string name);
private:
	MysqlMgr();
	MysqlDao  _dao;
};