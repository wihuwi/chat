#pragma once

#include "const.h"

class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;

class LogicSystem:public Singleton<LogicSystem>
{
	friend Singleton<LogicSystem>;
public:
	~LogicSystem() = default;
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);
	bool HandlePost(std::string, std::shared_ptr<HttpConnection>);
	void RegGet(std::string, HttpHandler);
	void RegPost(std::string, HttpHandler);

private:
	LogicSystem();

	std::map<std::string, HttpHandler> _get_handlers;
	std::map<std::string, HttpHandler> _post_handlers;
};

