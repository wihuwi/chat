#ifndef APPLYFRIENDITEM_H
#define APPLYFRIENDITEM_H

#include <QWidget>
#include "listitembase.h"
#include "userdata.h"

namespace Ui {
class ApplyFriendItem;
}

class ApplyFriendItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ApplyFriendItem(QWidget *parent = nullptr);
    ~ApplyFriendItem();
    void ShowAddBtn(bool show);
    void SetInfo(std::shared_ptr<ApplyInfo> info);
    int GetUid();
    QSize sizeHint() const override {
        return QSize(250, 80); // 返回自定义的尺寸
    }

private:
    Ui::ApplyFriendItem *ui;
    std::shared_ptr<ApplyInfo> _apply_info;
    bool _added;

signals:
    void sig_auth_friend(std::shared_ptr<ApplyInfo> info);
};

#endif // APPLYFRIENDITEM_H
