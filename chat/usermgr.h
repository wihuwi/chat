#ifndef USERMGR_H
#define USERMGR_H
#include "singleton.h"
#include "userdata.h"

class UserMgr: public Singleton<UserMgr>
{
    friend Singleton<UserMgr>;
public:
    ~UserMgr() = default;
    void SetUid(int uid);
    void SetName(QString name);
    void SetToken(QString token);
    QString GetName();
    int GetUid();
    void SetUserInfo(std::shared_ptr<UserInfo> userinfo);

    bool IsLoadChatFin();
    std::vector<std::shared_ptr<FriendInfo>> GetConListPerPage();
    void UpdateContactLoadedCount();
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();

    bool AlreadyApply(int uid);
    void AddApplyList(std::shared_ptr<ApplyInfo> apply);
    void AppendApplyList(QJsonArray array);

private:
    UserMgr();
    int _uid;
    QString _name;
    QString _token;
    std::shared_ptr<UserInfo> _user_info;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;
    QVector<std::shared_ptr<FriendInfo>> _friend_list;
    QMap<int, std::shared_ptr<FriendInfo>> _friend_map;
    int _chat_loaded;
    int _contact_loaded;
};

#endif // USERMGR_H
