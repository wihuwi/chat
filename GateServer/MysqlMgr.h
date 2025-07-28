#pragma once
#include "MysqlDao.h"
#include "Singleton.h"

class MysqlMgr:public Singleton<MysqlMgr>
{
	friend Singleton<MysqlMgr>;
public:
	~MysqlMgr();
	int RegUser(const std::string& name, const std::string& email, const std::string& passwd);
	bool UpdatePwd(const std::string& name, const std::string& pwd);
	bool CheckEmail(const std::string& name, const std::string& email);
private:
	MysqlMgr();

	MysqlDao _dao;
};

