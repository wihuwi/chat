#include "AsioIOServicePool.h"
#include <iostream>

AsioIOServicePool::AsioIOServicePool(int num): _ioServices(num), _works(num), _nextIOService(0) {
	for (int i = 0; i < num; i++) {
		_works[i] = std::make_unique<asio::executor_work_guard<asio::io_context::executor_type>>
			(asio::make_work_guard(_ioServices[i]));
	}

	for (int i = 0; i < num; i++) {
		_threads.emplace_back([this, i]() {
			this->_ioServices[i].run();
			});
	}
}

AsioIOServicePool::~AsioIOServicePool() {
	Stop();
	std::cout << "AsioIOServicePool destruct" << std::endl;
}

void AsioIOServicePool::Stop() {
	for (auto& work : _works) {
		work.reset();
	}

	for (auto& thread : _threads) {
		thread.join();
	}
}

asio::io_context& AsioIOServicePool::GetIOService() {
	asio::io_context& ioc = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return ioc;
}