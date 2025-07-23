#include "CServer.h"
#include "HttpConnection.h"

CServer::CServer(asio::io_context& ioc, unsigned short port):
	_ioc(ioc), _acceptor(_ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), _socket(_ioc) {

}

void CServer::Start() {
	auto self = shared_from_this();
	_acceptor.async_accept(_socket, [self](const beast::error_code& err) {
		try {
			if (err) {
				self->Start();
				return;
			}
			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
		}
		catch (std::exception& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}

		});
}

