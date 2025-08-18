#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include "listitembase.h"
#include "userdata.h"

namespace Ui {
class ChatuserWid;
}

class ChatuserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatuserWid(QWidget *parent = nullptr);
    ~ChatuserWid();
    QSize sizeHint() const override{
        return QSize(250,50);
    }
    void SetInfo(QString name, QString head, QString msg);
    void SetInfo(std::shared_ptr<UserInfo> info);
    void SetInfo(std::shared_ptr<FriendInfo> info);
    std::shared_ptr<UserInfo> GetInfo();
    void updateLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs);

private:
    Ui::ChatuserWid *ui;
    QString _name;
    QString _head;
    QString _msg;
    std::shared_ptr<UserInfo> _friend_info;
};

#endif // CHATUSERWID_H
