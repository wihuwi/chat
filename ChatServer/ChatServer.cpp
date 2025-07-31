
#include <iostream>
#include <boost/asio.hpp>
#include "CServer.h"
#include "ConfigMgr.h"

int main()
{
	try {
		auto config = ConfigMgr::Inst();
		auto pool = AsioIOServicePool::GetInstance();
		boost::asio::io_context  io_context;
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&io_context, pool](auto, auto) {
			io_context.stop();
			pool->Close();
			});
		CServer s(io_context, 8090);
		io_context.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

