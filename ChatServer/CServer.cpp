#include "CServer.h"
#include "CSession.h"

CServer::CServer( boost::asio::io_context& ioc, short port) :_ioc(ioc), _port(port),
_acceptor(_ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
	StartAccept();
}

CServer::~CServer() {

}

void CServer::StartAccept() {
	auto& ioc = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<CSession> session = std::make_shared<CSession>(ioc, this);
	_acceptor.async_accept(session->GetSocket(), 
		std::bind(&CServer::HandleAccept, this, session, std::placeholders::_1));
}

void CServer::HandleAccept(std::shared_ptr<CSession> session, const boost::system::error_code& ec) {
	if (!ec) {
		session->Start();
		std::lock_guard<std::mutex> lock(_mutex);
		_sessions[session->GetUuid()] = session;
	}
	else {
		std::cerr << "error:" << ec.what();
	}
	StartAccept();
}

void CServer::ClearSession(std::string uuid) {
	std::lock_guard<std::mutex> lock(_mutex);
	_sessions.erase(uuid);
}