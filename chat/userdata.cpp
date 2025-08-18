#include "userdata.h"
AddFriendApply::AddFriendApply(int from_uid, QString name, QString desc,QString icon, QString nick, int sex)
    :_from_uid(from_uid),_name(name),
    _desc(desc),_icon(icon),_nick(nick),_sex(sex)
{

}

void FriendInfo::AppendChatMsgs(const std::vector<std::shared_ptr<TextChatData>> text_vec)
{
    for(const auto & text: text_vec){
        _chat_msgs.push_back(text);
    }
}
