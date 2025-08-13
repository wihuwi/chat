#ifndef GROUPTIPITEM_H
#define GROUPTIPITEM_H

#include <QWidget>
#include "listitembase.h"

namespace Ui {
class GroupTipItem;
}

class GroupTipItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit GroupTipItem(QWidget *parent = nullptr);
    ~GroupTipItem();
    void SetGroupTip(QString str);
    QSize SizeHint();

private:
    Ui::GroupTipItem *ui;
};

#endif // GROUPTIPITEM_H
