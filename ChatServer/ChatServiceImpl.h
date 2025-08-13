#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using message::AddFriendReq;
using message::AddFriendRsp;
using message::AuthFriendReq;
using message::AuthFriendRsp;
using message::TextChatMsgReq;
using message::TextChatMsgRsp;

using message::ChatService;

class ChatServiceImpl final: public ChatService::Service
{
public:
	ChatServiceImpl();
	~ChatServiceImpl();
	virtual Status NotifyAddFriend(ServerContext* context, const AddFriendReq* request, AddFriendRsp* response);
	virtual Status NotifyAuthFriend(ServerContext* context, const AuthFriendReq* request, AuthFriendRsp* response);
	virtual Status NotifyTextChatMsg(ServerContext* context, const TextChatMsgReq* request, TextChatMsgRsp* response);

};

