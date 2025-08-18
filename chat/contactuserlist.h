#ifndef CONTACTUSERLIST_H
#define CONTACTUSERLIST_H
#include <QListWidget>
#include "grouptipitem.h"
#include "conuseritem.h"

class ContactUserList: public QListWidget
{
    Q_OBJECT
public:
    ContactUserList(QWidget* parent = nullptr);
    ~ContactUserList();
    void ShowRedPoint(bool showRed);

protected:
    bool eventFilter(QObject* obj, QEvent* event);

private:
    void addContactUserList();

    bool _load_pending;
    ConUserItem* _add_friend_item;
    QListWidgetItem * _groupitem;

signals:
    void sig_switch_apply_friend_page();
    void sig_switch_friend_info_page(std::shared_ptr<UserInfo>);
    void sig_loading_contact_user();
public slots:
    void slot_add_auth_friend(std::shared_ptr<AuthInfo> authinfo);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> authrsp);
private slots:
    void slot_item_clicked(QListWidgetItem *item);
};

#endif // CONTACTUSERLIST_H
