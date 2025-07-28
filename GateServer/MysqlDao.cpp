#include "MysqlDao.h"

MysqlPool::MysqlPool(const std::string& url, const std::string& user,
	const std::string& pass, const std::string& schema, int poolSize):
_url(url), _user(user), _pass(pass), _schema(schema), _poolSize(poolSize), _b_stop(false), _fail_count(0){
	std::cout << url << " " << user << " " << pass << " " << schema << std::endl;
	try {
		for (int i = 0; i < poolSize; i++) {
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
			auto con = driver->connect(url, user, pass);
			con->setSchema(schema);
			auto currentTime = std::chrono::system_clock::now().time_since_epoch();
			int64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
			
			_connections.push(std::make_unique<SqlConnection>(con, timestamp));
		}
	}
	catch (sql::SQLException& e) {
		std::cout << "mysql pool init err" << e.what() << std::endl;
	}
}

MysqlPool::~MysqlPool() {
	std::unique_lock<std::mutex> lock(_mutex);
	while (!_connections.empty()) {
		_connections.pop();
	}
}

std::unique_ptr<SqlConnection> MysqlPool::GetConnection() {
	std::unique_lock<std::mutex> lock(_mutex);
	_cond.wait(lock, [this]() {
		if (_b_stop) {
			return true;
		}
		return !_connections.empty();
		});

	if (_b_stop) {
		return nullptr;
	}

	auto con = std::move(_connections.front());
	_connections.pop();
	return con;
}

void MysqlPool::ReturnConnection(std::unique_ptr<SqlConnection> con) {
	std::lock_guard<std::mutex> lock(_mutex);
	if (_b_stop) {
		return;
	}

	_connections.push(std::move(con));
	_cond.notify_one();
}

void MysqlPool::CheckConnection() {
	int number = _connections.size();
	auto currentTime = std::chrono::system_clock::now().time_since_epoch();
	int64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
	for (int i = 0; i < number; i++) {
		auto con = std::move(_connections.front());
		_connections.pop();

		Defer defer([this, &con]() {
			_connections.push(std::move(con));
			});

		if (con->_last_oper_time - timestamp < 5) {
			//_connections.push(std::move(con));
			continue;
		}

		try {
			std::unique_ptr<sql::Statement> stat(con->_con->createStatement());
			stat->executeQuery("SELECT 1");
			con->_last_oper_time = timestamp;
		}
		catch (sql::SQLException& e) {
			std::cout << "check sql wrong:" << e.what() << std::endl;
			auto driver = sql::mysql::get_driver_instance();
			auto new_sql_con = driver->connect(_url, _user, _pass);
			new_sql_con->setSchema(_schema);
			con->_con = new_sql_con;
			con->_last_oper_time = timestamp;
		}
	}
}

void MysqlPool::CheckConnectionPro() {
	size_t number, processed = 0;
	{
		std::lock_guard<std::mutex> lock(_mutex);
		number = _connections.size();
	}

	auto currentTime = std::chrono::system_clock::now().time_since_epoch();
	int64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();

	while (processed < number) {
		std::unique_ptr<SqlConnection> con;
		{
			std::lock_guard<std::mutex>lock(_mutex);
			if (_connections.empty()) {
				break;
			}
			con = std::move(_connections.front());
			_connections.pop();
		}

		bool healthy = true;
		if (timestamp - con->_last_oper_time >= 5) {
			try {
				std::unique_ptr<sql::Statement> stat(con->_con->createStatement());
				stat->executeQuery("SELECT 1");
				con->_last_oper_time = timestamp;
			}
			catch (sql::SQLException& e) {
				std::cout << "check sql err: " << e.what() << std::endl;
				_fail_count++;
				healthy = false;
			}
		}

		//如果正常
		if (healthy) {
			std::lock_guard<std::mutex> lock(_mutex);
			_connections.push(std::move(con));
			_cond.notify_one();
		}
		processed++;
	}

	while (_fail_count) {
		if (ReConnect(timestamp)) {
			_fail_count--;
		}
		else {
			break;
			//why break  /use continue?
		}
	}
}

bool MysqlPool::ReConnect(int64_t timestamp) {
	try {
		auto driver = sql::mysql::get_driver_instance();
		auto con = driver->connect(_url, _user, _pass);
		con->setSchema(_schema);
		std::unique_ptr<SqlConnection> sql_con = std::make_unique<SqlConnection>(con, timestamp);
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_connections.push(std::move(sql_con));
		}
		return true;
	}
	catch (sql::SQLException& e) {
		std::cout << "ReConnect err: " << e.what() << std::endl;
		return false;
	}
}

void MysqlPool::Close() {
	_b_stop = true;
	_cond.notify_all();
}

MysqlDao::MysqlDao() {
	std::string host = ConfigMgr::GetInstance()["Mysql"]["Host"];
	std::string port = ConfigMgr::GetInstance()["Mysql"]["Port"];
	std::string user = ConfigMgr::GetInstance()["Mysql"]["User"];
	std::string passwd = ConfigMgr::GetInstance()["Mysql"]["Passwd"];
	std::string schema = ConfigMgr::GetInstance()["Mysql"]["Schema"];
	_pool.reset(new MysqlPool(host + ":" + port, user, passwd, schema, 5));
}

MysqlDao::~MysqlDao() {

}

int MysqlDao::RegUser(const std::string& name, const std::string& passwd, const std::string& email) {
	auto con = _pool->GetConnection();
	Defer([this, &con]() {
		_pool->ReturnConnection(std::move(con));
		});
	try {
		if (con == nullptr) {
			return -1;
		}

		std::unique_ptr<sql::PreparedStatement> pre_stst(con->_con->prepareStatement("CALL reg_user (?, ?, ?, @result)"));
		pre_stst->setString(1, name);
		pre_stst->setString(2, email);
		pre_stst->setString(3, passwd);

		pre_stst->execute();

		std::unique_ptr<sql::Statement> stat(con->_con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stat->executeQuery("SELECT @result AS result"));
		if (res->next()) {
			int result = res->getInt("result");
			std::cout << "Result: " << result << std::endl;
			return result;
		}
		return -1;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::CheckEmail(const std::string& name, const std::string& email) {
	auto con = _pool->GetConnection();
	Defer dfer([this, &con]() {
		_pool->ReturnConnection(std::move(con));
		});
	//为什么不用 move 因为内部已经 move ？
	try {
		if (con == nullptr) {
			std::cout << "GetConnection failed" << std::endl;
			return false;
		}
		std::unique_ptr<sql::PreparedStatement> pre_stat(con->_con->prepareStatement("Select email from user where name = ?"));
		pre_stat->setString(1, name);
		std::unique_ptr<sql::ResultSet> res(pre_stat->executeQuery());
		//这段逻辑是什么
		while (res->next()) {
			std::cout << "Check Email: " << res->getString("email") << std::endl;
			if (email != res->getString("email")) {
				return false;
			}
			return true;
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}

}

bool MysqlDao::UpdatePwd(const std::string& name, const std::string& pwd) {
	auto con = _pool->GetConnection();
	Defer dfer([this, &con]() {
		_pool->ReturnConnection(std::move(con));
		});
	try {
		if (con == nullptr) {
			std::cout << "GetConnection failed" << std::endl;
			return false;
		}
		std::unique_ptr<sql::PreparedStatement> pre_stat(con->_con->prepareStatement("Update user Set pwd = ? where name = ?"));
		pre_stat->setString(1, pwd);
		pre_stat->setString(2, name);

		int updateCount = pre_stat->executeUpdate();
		std::cout << "Updated rows: " << updateCount << std::endl;
		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userinfo) {
	auto con = _pool->GetConnection();

	Defer defer([this, &con]() {
		//为什么可以传&con，uniptr？？
		_pool->ReturnConnection(std::move(con));
		});

	try {
		if (con == nullptr) {
			return false;
		}
		std::unique_ptr<sql::PreparedStatement> pre_stat(con->_con->prepareStatement("select * from user where email = ?"));
		pre_stat->setString(1, email);
		std::unique_ptr<sql::ResultSet> res(pre_stat->executeQuery());

		std::string origin_pwd = "";
		// 遍历结果集
		while (res->next()) {
			origin_pwd = res->getString("pwd");
			// 输出查询到的密码
			std::cout << "Password: " << origin_pwd << std::endl;
			break;
		}
		if (pwd != origin_pwd) {
			return false;
		}

		userinfo.uid = res->getInt("uid");
		userinfo.email = email;
		userinfo.passwd = pwd;
		userinfo.name = res->getString("name");
		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}