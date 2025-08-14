#include "ChatServiceImpl.h"
#include "const.h"
#include "UserMgr.h"

ChatServiceImpl::ChatServiceImpl() {

}
ChatServiceImpl::~ChatServiceImpl() {

}

Status ChatServiceImpl::NotifyAddFriend(ServerContext* context, const AddFriendReq* request, AddFriendRsp* response) {
	auto touid = request->touid();
	auto session = UserMgr::GetInstance()->GetSession(touid);

	Defer defer([request, response]() {
		response->set_error(ErrorCodes::Success);
		response->set_applyuid(request->applyuid());
		response->set_touid(request->touid());
		});

	//????????????????????
	if (session == nullptr) {
		return Status::OK;
	}

	//????????????????????
	Json::Value  rtvalue;
	rtvalue["error"] = ErrorCodes::Success;
	rtvalue["applyuid"] = request->applyuid();
	rtvalue["name"] = request->name();
	rtvalue["desc"] = request->desc();
	rtvalue["icon"] = request->icon();
	rtvalue["sex"] = request->sex();
	rtvalue["nick"] = request->nick();

	std::string return_str = rtvalue.toStyledString();

	session->Send(return_str, ID_NOTIFY_ADD_FRIEND_REQ);
	return Status::OK;
}

Status ChatServiceImpl::NotifyAuthFriend(ServerContext* context, const AuthFriendReq* request, AuthFriendRsp* response) {
	return Status::OK;
}

Status ChatServiceImpl::NotifyTextChatMsg(ServerContext* context, const TextChatMsgReq* request, TextChatMsgRsp* response) {
	return Status::OK;
}

bool ChatServiceImpl::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo)
{
	return false;
}
