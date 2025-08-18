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
    virtual bool eventFilter(QObject* obj, QEvent* e);

private:
    bool _load_pending;
signals:
    void sig_loading_chat_user();
};

#endif // CHATUSERLIST_H
