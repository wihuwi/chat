#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
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
    void loadMoreChatUser();
    void loadMoreConUser();
    void UpdateChatMsg(std::vector<std::shared_ptr<TextChatData>> msgdata);

protected:
    bool eventFilter(QObject* o, QEvent* e) override;

private:
    void HandleGlobalMousePress(QMouseEvent* e);
    void ClearLabelState(StateWidget* wid);
    void AddLBGroup(StateWidget* wid);
    void ShowSearch(bool search);
    void SetSelectChatItem(int uid);
    void SetSelectChatPage(int uid);
    Ui::ChatDialog *ui;
    ChatUIMode _mode;
    ChatUIMode _state;
    bool _b_loading;
    QList<StateWidget*> _lb_list;
    QMap<int, QListWidgetItem*> _chat_items_added;
    int _cur_chat_uid;
    QWidget* _last_widget;

private slots:
    void slot_side_chat();
    void slot_side_contact();
    //void slot_loading_chat_user();
    void slot_text_changed(const QString& str);
    void slot_switch_apply_page();
public slots:
    void slot_loading_chat_user();
    void slot_loading_contact_user();
    void slot_friend_apply(std::shared_ptr<AddFriendApply>);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
    void slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info);
    void slot_jump_chat_item(std::shared_ptr<SearchInfo> si);
    void slot_friend_info_page(std::shared_ptr<UserInfo> si);
    void slot_jump_chat_item_from_infopage(std::shared_ptr<UserInfo> si);
    void slot_item_clicked(QListWidgetItem*);
    void slot_append_send_data_msg(std::shared_ptr<TextChatData> data);
    void slot_text_chat_msg(std::shared_ptr<TextChatMsg> data);
};

#endif // CHATDIALOG_H
