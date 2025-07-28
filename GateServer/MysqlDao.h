#pragma once
#include "const.h"
#include <thread>
#include <jdbc\mysql_driver.h>
#include <jdbc\mysql_connection.h>
#include <jdbc\cppconn\prepared_statement.h>
#include <jdbc\cppconn\resultset.h>
#include <jdbc\cppconn\statement.h>
#include <jdbc\cppconn\exception.h>

class SqlConnection {
public:
	SqlConnection(sql::Connection* con, int64_t last_time) :_con(con), _last_oper_time(last_time) {}
	sql::Connection* _con;
	int64_t _last_oper_time;
};

class MysqlPool
{
public:
	MysqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int poolSize);
	~MysqlPool();
	std::unique_ptr<SqlConnection> GetConnection();
	void ReturnConnection(std::unique_ptr<SqlConnection>);
	void CheckConnection();
	void CheckConnectionPro();
	bool ReConnect(int64_t timestamp);
	void Close();

private:
	std::string _url;
	std::string _user;
	std::string _pass;
	std::string _schema;
	int _poolSize;
	std::mutex _mutex;
	std::condition_variable _cond;
	std::queue<std::unique_ptr<SqlConnection>> _connections;
	std::atomic<bool> _b_stop;
	std::thread _check_thread;
	int _fail_count;
};

struct UserInfo {
	std::string name;
	std::string passwd;
	int uid;
	std::string email;
};

class MysqlDao {
public:
	MysqlDao();
	~MysqlDao();
	int RegUser(const std::string& name, const std::string& passwd, const std::string& email);
	bool CheckEmail(const std::string& name, const std::string& email);
	bool UpdatePwd(const std::string& name, const std::string& pwd);
	bool CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userinfo);

private:
	std::unique_ptr<MysqlPool> _pool;
};