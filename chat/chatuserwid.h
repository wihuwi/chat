#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include "listitembase.h"

namespace Ui {
class ChatuserWid;
}

class ChatuserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatuserWid(QWidget *parent = nullptr);
    ~ChatuserWid();
    QSize sizeHint() const override{
        return QSize(250,50);
    }
    void SetInfo(QString name, QString head, QString msg);

private:
    Ui::ChatuserWid *ui;
    QString _name;
    QString _head;
    QString _msg;
};

#endif // CHATUSERWID_H
