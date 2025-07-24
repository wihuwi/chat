#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOServicePool.h"

CServer::CServer(asio::io_context& ioc, unsigned short port):
	_ioc(ioc), _acceptor(_ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {

}

void CServer::Start() {
	auto self = shared_from_this();
	asio::io_context& ioc = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> con = std::make_shared<HttpConnection>(ioc);
	_acceptor.async_accept(con->GetSocket(), [con, self](const beast::error_code& err) {
		try {
			if (err) {
				self->Start();
				return;
			}
			con->Start();
			self->Start();

		}
		catch (std::exception& e) {
			std::cout << "Error: " << e.what() << std::endl;
			self->Start();

		}

		});
}

