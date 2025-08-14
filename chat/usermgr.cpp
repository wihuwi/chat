#include "usermgr.h"
#include <QJsonValue>

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

void UserMgr::SetUserInfo(std::shared_ptr<UserInfo> userinfo)
{
    _user_info = userinfo;
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

bool UserMgr::AlreadyApply(int uid)
{
    for(auto& it: _apply_list){
        if(it->_uid == uid){
            return true;
        }
    }
    return false;
}

void UserMgr::AddApplyList(std::shared_ptr<ApplyInfo> apply)
{
    _apply_list.push_back(apply);
}

void UserMgr::AppendApplyList(QJsonArray array)
{
    // 遍历 QJsonArray 并输出每个元素
    for (const QJsonValue &value : array) {
        auto name = value["name"].toString();
        auto desc = value["desc"].toString();
        auto icon = value["icon"].toString();
        auto nick = value["nick"].toString();
        auto sex = value["sex"].toInt();
        auto uid = value["uid"].toInt();
        auto status = value["status"].toInt();
        auto info = std::make_shared<ApplyInfo>(uid, name,
                                                desc, icon, nick, sex, status);
        _apply_list.push_back(info);
    }
}
