#ifndef APPLYFRIENDPAGE_H
#define APPLYFRIENDPAGE_H

#include <QDialog>
#include "userdata.h"
#include "applyfrienditem.h"

namespace Ui {
class ApplyFriendPage;
}

class ApplyFriendPage : public QDialog
{
    Q_OBJECT

public:
    explicit ApplyFriendPage(QWidget *parent = nullptr);
    ~ApplyFriendPage();
    void AddNewApply(std::shared_ptr<AddFriendApply> apply);

protected:
    void paintEvent(QPaintEvent* event);

private:
    void LoadApplyList();

    Ui::ApplyFriendPage *ui;
    QMap<int, ApplyFriendItem*> _unauth_items;

public slots:
    void slot_auth_rsp(std::shared_ptr<AuthRsp> );
signals:
    void sig_show_search(bool);

};

#endif // APPLYFRIENDPAGE_H
