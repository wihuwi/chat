#include "chatuserwid.h"
#include "ui_chatuserwid.h"
#include "global.h"

ChatuserWid::ChatuserWid(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ChatuserWid)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CHAT_USER_ITEM);
}

ChatuserWid::~ChatuserWid()
{
    delete ui;
}

void ChatuserWid::SetInfo(QString name, QString head, QString msg){
    _name = name;
    _head = head;
    _msg = msg;

    // 加载图片
    QPixmap pixmap(_head);
    // 设置图片自动缩放
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);
    ui->user_name_lb->setText(_name);
    ui->user_chat_lb->setText(_msg);
}

void ChatuserWid::SetInfo(std::shared_ptr<UserInfo> info)
{
    _friend_info = info;

    QPixmap pix(QPixmap(info->_icon));

    ui->icon_lb->setPixmap(pix.scaled(ui->icon_lb->sizeHint(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->user_chat_lb->setText(info->_last_msg);
    ui->user_name_lb->setText(info->_name);
}

void ChatuserWid::SetInfo(std::shared_ptr<FriendInfo> friend_info)
{
    _friend_info = std::make_shared<UserInfo>(friend_info);
    // 加载图片
    QPixmap pixmap(_friend_info->_icon);

    // 设置图片自动缩放
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->user_name_lb->setText(_friend_info->_name);
    ui->user_chat_lb->setText(_friend_info->_last_msg);
}

std::shared_ptr<UserInfo> ChatuserWid::GetInfo()
{
    return _friend_info;
}

void ChatuserWid::updateLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs) {

    QString last_msg = "";
    for (auto& msg : msgs) {
        last_msg = msg->_msg_content;
        _friend_info->_chat_msgs.push_back(msg);
    }

    _friend_info->_last_msg = last_msg;
    ui->user_chat_lb->setText(_friend_info->_last_msg);
}
