#include "ChatServiceImpl.h"


ChatServiceImpl::ChatServiceImpl() {

}
ChatServiceImpl::~ChatServiceImpl() {

}
Status ChatServiceImpl::NotifyAddFriend(ServerContext* context, const AddFriendReq* request, AddFriendRsp* response) {
	return Status::OK;
}
Status ChatServiceImpl::NotifyAuthFriend(ServerContext* context, const AuthFriendReq* request, AuthFriendRsp* response) {
	return Status::OK;
}
Status ChatServiceImpl::NotifyTextChatMsg(ServerContext* context, const TextChatMsgReq* request, TextChatMsgRsp* response) {
	return Status::OK;
}