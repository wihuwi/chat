#ifndef CONUSERITEM_H
#define CONUSERITEM_H

#include "userdata.h"
#include "listitembase.h"
#include <QWidget>

namespace Ui {
class ConUserItem;
}

class ConUserItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ConUserItem(QWidget *parent = nullptr);
    ~ConUserItem();
    QSize sizeHint() const;
    void SetInfo(std::shared_ptr<AuthInfo> info);
    void SetInfo(std::shared_ptr<AuthRsp> info);
    void SetInfo(int uid, QString name, QString icon);
    void SetRedPoint(bool redPoint);
    std::shared_ptr<UserInfo> GetInfo();

private:
    Ui::ConUserItem *ui;
    std::shared_ptr<UserInfo> _info;
};

#endif // CONUSERITEM_H
