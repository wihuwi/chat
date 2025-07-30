#pragma once

#include "singleton.h"

#include <mutex>
#include <thread>
#include <boost/asio.hpp>
#include <vector>

//
class AsioIOServicePool :public Singleton<AsioIOServicePool>
{
	friend Singleton<AsioIOServicePool>;
	using Context = boost::asio::io_context;
	using Work = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
	using WorkPtr = std::unique_ptr<Work>;
public:
	~AsioIOServicePool();
	Context& GetIOService();
	void Close();

private:
	AsioIOServicePool(int poolSize = std::thread::hardware_concurrency());

	std::vector<Context> _ioServices;
	std::vector<WorkPtr> _works;
	std::vector<std::thread> _threads;
	int _poolSize;
	int _nextIOService;
	std::mutex _mutex;
};

