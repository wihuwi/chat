#include "MysqlMgr.h"

MysqlMgr::~MysqlMgr() {

}

int MysqlMgr::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
	return _dao.RegUser(name, email, pwd);
}

bool MysqlMgr::CheckEmail(const std::string& name, const std::string& email) {
	return _dao.CheckEmail(name, email);
}

bool MysqlMgr::UpdatePwd(const std::string& name, const std::string& pwd) {
	return _dao.UpdatePwd(name, pwd);
}

MysqlMgr::MysqlMgr() {
}

bool MysqlMgr::CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo) {
	return _dao.CheckPwd(name, pwd, userInfo);
}

bool MysqlMgr::AddFriendApply(const int& from_uid, const int& to_uid)
{
	return _dao.AddFriendApply(from_uid, to_uid);
}

bool MysqlMgr::GetApplyList(int uid, std::vector<std::shared_ptr<ApplyInfo>>& list, int begin, int limit)
{
	return _dao.GetApplyList(uid, list, begin, limit);
}

bool MysqlMgr::AuthFriendApply(int from, int to)
{
	return _dao.AuthFriendApply(from, to);
}

bool MysqlMgr::AddFriend(int from, int to, std::string backname)
{
	return _dao.AddFriend(from, to, backname);
}

bool MysqlMgr::GetFriendList(int uid, std::vector<std::shared_ptr<UserInfo>>& list)
{
	return _dao.GetFriendList(uid, list);
}

std::shared_ptr<UserInfo> MysqlMgr::GetUser(int uid) {
	return _dao.GetUser(uid);
}

std::shared_ptr<UserInfo> MysqlMgr::GetUser(std::string name) {
	return _dao.GetUser(name);
}
