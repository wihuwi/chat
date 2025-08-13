#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H

#include <QWidget>
#include "global.h"

class ListItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ListItemBase(QWidget *parent = nullptr);
    ~ListItemBase() = default;
    void SetItemType(ListItemType type);
    ListItemType GetItemType();

protected:

private:
    ListItemType _itemType;

signals:
};

#endif // LISTITEMBASE_H
