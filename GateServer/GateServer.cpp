
#include <iostream>
#include "CServer.h"
#include "ConfigMgr.h"
#include "const.h"
#include "RedisMgr.h"



int main()
{
    ConfigMgr& cfgMgr = ConfigMgr::GetInstance();
    short port = atoi(cfgMgr["GateServer"]["Port"].c_str());
    std::cout << port;
    try {
        asio::io_context ioc;
        unsigned short port = 3333;
        asio::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const system::error_code& err, int number) {
            if (err) {
                return;
            }
            ioc.stop();
            });
        std::shared_ptr<CServer>(new CServer(ioc, port))->Start();
        ioc.run();
    }
    catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

