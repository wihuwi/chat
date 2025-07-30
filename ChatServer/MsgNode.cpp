#include "MsgNode.h"

RecvNode::RecvNode(short maxlen, short msg_id):MsgNode(maxlen), _msg_id(msg_id) {

}

SendNode::SendNode(const char* data, short maxlen, short msg_id):MsgNode(maxlen + HEAD_TOTAL_LEN), _msg_id(msg_id) {
	short msg_id_net = boost::asio::detail::socket_ops::host_to_network_short(msg_id);
	short len_net = boost::asio::detail::socket_ops::host_to_network_short(maxlen);
	memcpy(_data, &msg_id_net, HEAD_ID_LEN);
	memcpy(_data + HEAD_ID_LEN, &len_net, HEAD_DATA_LEN);
	memcpy(_data + HEAD_TOTAL_LEN, data, maxlen);
}

SendNode::SendNode(short maxlen, short msg_id) :MsgNode(maxlen + HEAD_TOTAL_LEN), _msg_id(msg_id) {
	short msg_id_net = boost::asio::detail::socket_ops::host_to_network_short(msg_id);
	short len_net = boost::asio::detail::socket_ops::host_to_network_short(maxlen);
	memcpy(_data, &msg_id_net, HEAD_ID_LEN);
	memcpy(_data + HEAD_ID_LEN, &len_net, HEAD_DATA_LEN);
	_cur_len += HEAD_TOTAL_LEN;
}