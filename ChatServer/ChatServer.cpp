#include <iostream>
#include <boost/asio.hpp>
#include "CServer.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include "ChatServiceImpl.h"

int main()
{
	auto& inst = ConfigMgr::Inst();
	std::string server_name = ConfigMgr::Inst()["SelfServer"]["Name"];
	try {
		auto pool = AsioIOServicePool::GetInstance();
		//服务器初始化登录人数
		RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, "0");
		Defer defer([server_name]() {
			RedisMgr::GetInstance()->HDel(LOGIN_COUNT, server_name);
			RedisMgr::GetInstance()->Close();
			});
		//chatService 开启
		ServerBuilder builder;
		ChatServiceImpl chatService;

		std::string server_address(inst["SelfServer"]["Host"] + ":" + inst["SelfServer"]["RPCPort"]);
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		builder.RegisterService(&chatService);
		std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
		std::cout << "RPC Server listening on " << server_address << std::endl;
		std::thread grpc_server_thread([&server]() {
			server->Wait();
			});

		//主线程开启
		boost::asio::io_context  io_context;
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&io_context, pool, &server](auto, auto) {
			io_context.stop();
			pool->Close();
			server->Shutdown();
			});
		CServer s(io_context, 8090);
		io_context.run();

		grpc_server_thread.join();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

