#include "CSession.h"
#include "CServer.h"
#include "LogicSystem.h"

CSession::CSession(boost::asio::io_context& ioc, CServer* server) :_socket(ioc), _server(server){
	boost::uuids::uuid uid = boost::uuids::random_generator()();
	_session_id = boost::uuids::to_string(uid);

	_recv_head_node = std::make_shared<RecvNode>(HEAD_TOTAL_LEN, 0);
}

CSession::~CSession() {

}

boost::asio::ip::tcp::socket& CSession::GetSocket() {
	return _socket;
}

std::string CSession::GetSessionId() {
	return _session_id;
}

void CSession::SetUserId(int uid) {
	_user_uid = uid;
}

int CSession::GetUserId() {
	return _user_uid;
}

void CSession::Start() {
	AsyncReadHead(HEAD_TOTAL_LEN);
	//只会读取不超过HEAD_TOTAL_LEN
}

void CSession::AsyncReadHead(int len) {
	auto self = shared_from_this();
	
	AsyncReadFull(len, [this, self](const boost::system::error_code& errcode, std::size_t bytes_transferred) {
		try {
			if (errcode) {
				std::cout << "handle read failed, error is " << errcode.what() << std::endl;
				self->Close();
				self->_server->ClearSession(self->_session_id);
				return;
			}

			//下面这段有必要吗，按逻辑来讲，之后errcode & bytes足够了才返回
			if (bytes_transferred < HEAD_TOTAL_LEN) {
				std::cout << "read length not match, read [" << bytes_transferred << "] , total ["
					<< HEAD_TOTAL_LEN << "]" << std::endl;
				self->Close();
				self->_server->ClearSession(self->_session_id);
				return;
			}

			_recv_head_node->Clear();
			memcpy(_recv_head_node->_data, _data, bytes_transferred);

			//获取头部MSGID数据
			short msg_id = 0;
			memcpy(&msg_id, _recv_head_node->_data, HEAD_ID_LEN);
			//网络字节序转化为本地字节序
			msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
			std::cout << "msg_id is " << msg_id << std::endl;
			//id非法
			if (msg_id > MAX_LENGTH) {
				std::cout << "invalid msg_id is " << msg_id << std::endl;
				_server->ClearSession(_session_id);
				return;
			}
			short msg_len = 0;
			memcpy(&msg_len, _recv_head_node->_data + HEAD_ID_LEN, HEAD_DATA_LEN);
			//网络字节序转化为本地字节序
			msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
			std::cout << "msg_len is " << msg_len << std::endl;

			//id非法
			if (msg_len > MAX_LENGTH) {
				std::cout << "invalid data length is " << msg_len << std::endl;
				_server->ClearSession(_session_id);
				return;
			}

			_recv_msg_node = std::make_shared<RecvNode>(msg_len, msg_id);
			AsyncReadBody(msg_len);
		}
		catch (std::exception& e) {
			std::cout << "Exception code is " << e.what() << std::endl;
		}
		});
}

void CSession::AsyncReadBody(int len) {
	auto self = shared_from_this();
	AsyncReadFull(len, [self, len](const boost::system::error_code& errcode, std::size_t bytes_transferred) {
		try {
			if (errcode) {
				std::cerr << "Read Full Err: " << errcode.what() << std::endl;
				self->_server->ClearSession(self->_session_id);
				self->Close();
				return;
			}

			if (bytes_transferred < len) {
				std::cerr << "read body length err: " << bytes_transferred << "|" << len << std::endl;
				self->_server->ClearSession(self->_session_id);
				self->Close();
				return;
			}

			memcpy(self->_recv_msg_node->_data, self->_data, bytes_transferred);
			self->_recv_msg_node->_cur_len += bytes_transferred;
			self->_recv_msg_node->_data[self->_recv_msg_node->_total_len] = '\0';
			std::cout << "receive data is " << self->_recv_msg_node->_data << std::endl;
			//此处将消息投递到逻辑队列中
			LogicSystem::GetInstance()->PostMsgToQue(
				std::make_shared<LogicNode>(self->shared_from_this(), self->_recv_msg_node));
			//继续监听头部接受事件
			self->AsyncReadHead(HEAD_TOTAL_LEN);
		}
		catch (std::exception& e) {
			std::cout << "Exception code is " << e.what() << std::endl;
		}
		});
}

void CSession::AsyncReadFull(int len, 
	std::function<void(const boost::system::error_code& errcode, std::size_t bytes_transferred)> handler){
	memset(_data, '\0', MAX_LENGTH);
	AsyncReadLen(0, len, handler);
}

void CSession::AsyncReadLen(int read_len, int total_len, 
	std::function<void(const boost::system::error_code& errorcode, std::size_t bytes_transferred)> handler) {
	auto self = shared_from_this();
	_socket.async_read_some(boost::asio::buffer(_data + read_len, total_len - read_len),
		[read_len, total_len, handler, self](const boost::system::error_code& ec, std::size_t  bytesTransfered) {
			if (ec) {
				// 出现错误，调用回调函数
				handler(ec, read_len + bytesTransfered);
				return;
			}

			if (read_len + bytesTransfered >= total_len) {
				//长度够了就调用回调函数
				handler(ec, read_len + bytesTransfered);
				return;
			}

			// 没有错误，且长度不足则继续读取
			self->AsyncReadLen(read_len + bytesTransfered, total_len, handler);
		});
}

void CSession::Close() {
	_socket.close();
}

void CSession::Send(const std::string& data, short msg_id) {
	std::lock_guard<std::mutex> lock(_que_mutex);
	if (_msg_que.size() >= MAX_SENDQUE) {
		std::cout << "session: " << _session_id << " send que fulled, size is " << MAX_SENDQUE << std::endl;
		return;
	}
	_msg_que.push(std::shared_ptr<SendNode>(new SendNode(data.c_str(), data.length(), msg_id)));
	if (_msg_que.size() > 1) {
		return;
	}

	auto& node = _msg_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(node->_data, node->_total_len),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));
}

void CSession::Send(const char* data, short len, short msg_id) {
	std::lock_guard<std::mutex> lcok(_que_mutex);
	if (_msg_que.size() > MAX_SENDQUE) {

	}
	_msg_que.push(std::shared_ptr<SendNode>(new SendNode(data, len, msg_id)));
	if (_msg_que.size() > 1) {
		return;
	}

	auto node = _msg_que.front();
	boost::asio::async_write(_socket, boost::asio::buffer(node->_data, node->_total_len),
		std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));
}

void CSession::HandleWrite(const boost::system::error_code& errcode, std::shared_ptr<CSession> session) {
	//增加异常处理
	try {
		if (!errcode) {
			std::lock_guard<std::mutex> lock(_que_mutex);
			//cout << "send data " << _send_que.front()->_data+HEAD_LENGTH << endl;
			_msg_que.pop();
			if (!_msg_que.empty()) {
				auto& msgnode = _msg_que.front();
				boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
					std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_from_this()));
			}
		}
		else {
			std::cout << "handle write failed, error is " << errcode.what() << std::endl;
			Close();
			_server->ClearSession(_session_id);
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception code : " << e.what() << std::endl;
	}
}