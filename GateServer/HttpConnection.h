#pragma once

#include "const.h"
#include <iostream>

class LogicSystem;

class HttpConnection: public std::enable_shared_from_this<HttpConnection>
{
	friend LogicSystem;
public:
	HttpConnection(asio::io_context& ioc);
	~HttpConnection() = default;
	void Start();
	asio::ip::tcp::socket& GetSocket();
private:
	void PreParseGetParam();
	void CheckDeadLine();
	void WriteResponse();
	void HandleReq();

	asio::ip::tcp::socket _socket;
	beast::http::request<beast::http::dynamic_body> _request;
	beast::http::response<beast::http::dynamic_body> _response;
	beast::flat_buffer _buffer{ 8192 };
	asio::steady_timer _deadline{ _socket.get_executor(), std::chrono::seconds(60) };

	std::string _get_url;
	std::unordered_map < std::string, std::string>_get_params;
};

