#pragma once

#include "const.h"

using namespace boost;

class CServer: public std::enable_shared_from_this<CServer>{
public:
	CServer(asio::io_context&, unsigned short port);
	~CServer() = default;
	void Start();
private:
	asio::io_context& _ioc;
	asio::ip::tcp::acceptor _acceptor;
	asio::ip::tcp::socket _socket;
};

