#include "ChatGrpcClient.h"

ChatGrpcClient::ChatGrpcClient() {
	auto inst = ConfigMgr::Inst();
	auto server_list = inst["PeerServer"]["Servers"];

	std::vector<std::string> words;
	std::stringstream ss(server_list);
	std::string word;
	while (std::getline(ss, word, ',')) {
		words.push_back(word);
	}

	for (auto& word : words) {
		if (inst[word]["Name"].empty()) {
			continue;
		}
		_pools[inst[word]["Name"]] = std::make_unique<ChatConPool>(5, inst[word]["Host"], inst[word]["Port"]);
	}
}

ChatGrpcClient::~ChatGrpcClient() {

}

AddFriendRsp ChatGrpcClient::NotifyAddFriend(AddFriendReq) {
	AddFriendRsp rsp;
	return rsp;
}
AuthFriendRsp ChatGrpcClient::RplyAddFriend(AuthFriendReq) {
	AuthFriendRsp rsp;
	return rsp;
}
TextChatMsgRsp ChatGrpcClient::NotifyTextChatMsg(TextChatMsgReq) {
	TextChatMsgRsp rsp;
	return rsp;
}
bool ChatGrpcClient::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo) {
	return true;
}