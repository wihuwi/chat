#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"
#include "statewidget.h"
#include "userdata.h"

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
    void addChatUserList();

protected:
    bool eventFilter(QObject* o, QEvent* e) override;

private:
    void HandleGlobalMousePress(QMouseEvent* e);
    void ClearLabelState(StateWidget* wid);
    void AddLBGroup(StateWidget* wid);
    void ShowSearch(bool search);
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    QList<StateWidget*> _lb_list;


private slots:
    void slot_side_chat();
    void slot_side_contact();
    //void slot_loading_chat_user();
    void slot_text_changed(const QString& str);
    void slot_switch_apply_page();
public slots:
    void slot_friend_apply(std::shared_ptr<AddFriendApply>);
};

#endif // CHATDIALOG_H
