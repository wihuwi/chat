#include "AsioIOServicePool.h"
#include <iostream>

AsioIOServicePool::AsioIOServicePool(int poolSize):
	_ioServices(poolSize), _works(poolSize), _poolSize(poolSize), _nextIOService(0){
	for (int i = 0; i < poolSize; i++) {
		_works[i] = std::make_unique<Work>(boost::asio::make_work_guard(_ioServices[i].get_executor()));
	}

	for (int i = 0; i < poolSize; i++) {
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	std::lock_guard<std::mutex> lock(_mutex);
	auto& ioc = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return ioc;
}

void AsioIOServicePool::Close() {
	for (int i = 0; i < _poolSize; i++) {
		_works[i].reset();
	}
	for (int i = 0; i < _poolSize; i++) {
		_threads[i].join();
	}
}

AsioIOServicePool::~AsioIOServicePool() {
	std::cout << "AsioIOServicePool destruct" << std::endl;
}