#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include <memory>
#include "userdata.h"

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();
    void SetUserInfo(std::shared_ptr<UserInfo> info);
    void AppendChatMsg(std::shared_ptr<TextChatData> msg);
protected:
    //需要吗？
    virtual void paintEvent(QPaintEvent* event);

private slots:
    void on_send_btn_clicked();

private:
    Ui::ChatPage *ui;
    std::shared_ptr<UserInfo> _user_info;
signals:
    void sig_append_send_chat_msg(std::shared_ptr<TextChatData>);
};

#endif // CHATPAGE_H
