#pragma once
#include <boost/asio.hpp>
#include <mutex>
#include <memory>
#include <iostream>
#include <map>
#include "Csession.h"
#include "AsioIOServicePool.h"

class CSession;
class CServer
{
public:
	CServer(boost::asio::io_context& _ioc, short port);
	~CServer();
	void ClearSession(std::string uuid);
private:
	void StartAccept();
	void HandleAccept(std::shared_ptr<CSession> session, const boost::system::error_code& ec);
	boost::asio::io_context& _ioc;
	boost::asio::ip::tcp::acceptor _acceptor;
	short _port;
	std::map<std::string, std::shared_ptr<CSession>> _sessions;
	std::mutex _mutex;
};

