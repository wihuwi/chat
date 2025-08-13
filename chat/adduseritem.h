#ifndef ADDUSERITEM_H
#define ADDUSERITEM_H
#include "listitembase.h"
#include <QWidget>

namespace Ui {
class AddUserItem;
}

class AddUserItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit AddUserItem(QWidget *parent = nullptr);
    ~AddUserItem();
    QSize sizeHint() const override{
        return QSize(250, 50);
    }

private:
    Ui::AddUserItem *ui;
};

#endif // ADDUSERITEM_H
