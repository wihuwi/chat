#pragma once
#include <boost/asio.hpp>
#include <boost/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <queue>
#include <functional>
#include <memory>
#include <iostream>

#include "MsgNode.h"

class CServer;
class CSession: public std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::io_context& ioc, CServer* server);
	~CSession();
	boost::asio::ip::tcp::socket& GetSocket();
	std::string GetUuid();
	void Start();
	void AsyncReadHead(int len);
	void AsyncReadFull(int len, std::function<void(const boost::system::error_code& errcode, std::size_t bytes_transferred)>handler);
	void AsyncReadBody(int len);
	void AsyncReadLen(int readLen, int maxLen, std::function<void(const boost::system::error_code& errcode, std::size_t bytes_transferred)>handler);

	void Send(const std::string& data, short msg_id);
	void Send(const char* data, short len, short msg_id);
	void HandleWrite(const boost::system::error_code& errcode, std::shared_ptr<CSession> session);
	void Close();
private:
	boost::asio::ip::tcp::socket _socket;
	std::string _uuid;
	char _data[MAX_LENGTH];
	std::queue<std::shared_ptr<SendNode>> _msg_que;
	std::mutex _que_mutex;
	std::shared_ptr<RecvNode> _recv_head_node;
	std::shared_ptr<RecvNode> _recv_msg_node;
	std::shared_ptr<SendNode> _send_node;
	CServer* _server;
};

class LogicNode {
public:
	LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<SendNode> node) :_session(session), _node(node) {}

	std::shared_ptr<CSession> _session;
	std::shared_ptr<SendNode> _node;
};