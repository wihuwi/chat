#pragma once
#include "const.h"
#include "Singleton.h"

class AsioIOServicePool:public Singleton<AsioIOServicePool>
{
	friend Singleton<AsioIOServicePool>;
public:
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	void Stop();
	asio::io_context& GetIOService();

private:
	AsioIOServicePool(int num = std::thread::hardware_concurrency());

	std::vector<std::thread> _threads;
	std::vector<asio::io_context> _ioServices;
	std::vector<std::unique_ptr<asio::executor_work_guard<asio::io_context::executor_type>>> _works;
	std::size_t _nextIOService;
};

