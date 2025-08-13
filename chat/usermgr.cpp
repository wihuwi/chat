#include "usermgr.h"

UserMgr::UserMgr():_user_info(nullptr), _chat_loaded(0),_contact_loaded(0)
{

}

void UserMgr::SetUid(int uid){
    _uid = uid;
}

void UserMgr::SetName(QString name){
    _name = name;
}

void UserMgr::SetToken(QString token){
    _token = token;
}

QString UserMgr::GetName()
{
    return _name;
}

int UserMgr::GetUid()
{
    return _uid;
}

bool UserMgr::IsLoadChatFin() {
    if (_chat_loaded >= _friend_list.size()) {
        return true;
    }

    return false;
}

std::vector<std::shared_ptr<FriendInfo> > UserMgr::GetConListPerPage()
{
    std::vector<std::shared_ptr<FriendInfo>> friend_list;
    int begin = _contact_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;

    if (begin >= _friend_list.size()) {
        return friend_list;
    }

    if (end > _friend_list.size()) {
        friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin, _friend_list.end());
        return friend_list;
    }


    friend_list = std::vector<std::shared_ptr<FriendInfo>>(_friend_list.begin() + begin, _friend_list.begin() + end);
    return friend_list;
}

void UserMgr::UpdateContactLoadedCount()
{
    int begin = _contact_loaded;
    int end = begin + CHAT_COUNT_PER_PAGE;

    if (begin >= _friend_list.size()) {
        return;
    }

    if (end > _friend_list.size()) {
        _contact_loaded = _friend_list.size();
        return;
    }

    _contact_loaded = end;

}

std::vector<std::shared_ptr<ApplyInfo> > UserMgr::GetApplyList()
{
    return _apply_list;
}
