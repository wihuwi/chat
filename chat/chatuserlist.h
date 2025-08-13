#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H
#include <QListWidget>
#include <QWheelEvent>
#include <QScrollBar>

class ChatUserList: public QListWidget
{
    Q_OBJECT
public:
    ChatUserList(QWidget* parent = nullptr);
    ~ChatUserList();
protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

signals:
    void sig_loading_chat_user();
};

#endif // CHATUSERLIST_H
