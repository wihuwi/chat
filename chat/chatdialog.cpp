#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QAction>
#include <QRandomGenerator>
#include <QPixmap>
#include "customizeedit.h"
#include "chatuserwid.h"
#include "tcpmgr.h"
#include "usermgr.h"

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog), _mode(ChatUIMode::ChatMode), _state(ChatUIMode::ChatMode), _b_loading(false), _cur_chat_uid(0)
    , _last_widget(nullptr)
{
    ui->setupUi(this);
    ui->add_btn->SetState("normal", "normal", "hover");

    ui->search_edit->SetMaxLen(21);

    QAction* searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/icons/search.png"));
    ui->search_edit->addAction(searchAction, CustomizeEdit::ActionPosition::LeadingPosition);
    QAction* clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/icons/close_transparent.png"));
    ui->search_edit->addAction(clearAction, CustomizeEdit::ActionPosition::TrailingPosition);
    connect(clearAction, &QAction::triggered, ui->search_edit, [&](){
        ui->search_edit->setText("");
        ShowSearch(false);
    });
    connect(ui->search_edit, &QLineEdit::textChanged, clearAction, [clearAction](const QString& text){
        if(text.isEmpty()){
            clearAction->setIcon(QIcon(":/icons/close_transparent.png"));
        }else{
            clearAction->setIcon(QIcon(":/icons/close_search.png"));
        }
    });

    ShowSearch(false);
    addChatUserList();

    qApp->installEventFilter(this);

    //连接加载信号
    connect(ui->chat_user_list, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);
    connect(ui->con_user_list, &ContactUserList::sig_loading_contact_user, this, &ChatDialog::slot_loading_contact_user);

    //设置初始界面
    ui->stackedWidget->setCurrentWidget(ui->chat_page);

    //搜索框
    connect(ui->search_edit, &QLineEdit::textChanged, this, &ChatDialog::slot_text_changed);

    //侧边栏
    QPixmap pixmap = QPixmap(UserMgr::GetInstance()->GetIcon());
    ui->head_lb->setPixmap(pixmap.scaled(QSize(40, 40), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->head_lb->setScaledContents(true);

    qDebug()<<UserMgr::GetInstance()->GetIcon();
    ui->side_chat_lb->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");
    ui->side_contact_lb->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");

    AddLBGroup(ui->side_chat_lb);
    AddLBGroup(ui->side_contact_lb);

    connect(ui->side_chat_lb, &StateWidget::clicked, this, &ChatDialog::slot_side_chat);
    connect(ui->side_contact_lb, &StateWidget::clicked, this, &ChatDialog::slot_side_contact);
    ui->side_chat_lb->SetSelected(true);

    connect(ui->con_user_list, &ContactUserList::sig_switch_apply_friend_page, this, &ChatDialog::slot_switch_apply_page);

    //设置searchlist的 search_edit
    ui->search_list->SetSearchEdit(ui->search_edit);

    //连接添加好友通知回包 ->
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_friend_apply, this, &ChatDialog::slot_friend_apply);

    //连接完成添加
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_auth_rsp, this, &ChatDialog::slot_auth_rsp);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_add_auth_friend, this, &ChatDialog::slot_add_auth_friend);

    //聊天跳转
    connect(ui->search_list, &SearchList::sig_jump_chat_item, this, &ChatDialog::slot_jump_chat_item);

    //连接点击联系人item发出的信号和用户信息展示槽函数
    connect(ui->con_user_list, &ContactUserList::sig_switch_friend_info_page,
            this,&ChatDialog::slot_friend_info_page);

    //连接用户信息界面跳转聊天
    connect(ui->friend_info_page, &FriendInfoPage::sig_jump_chat_item, this, &ChatDialog::slot_jump_chat_item_from_infopage);

    connect(ui->chat_user_list, &ChatUserList::itemClicked, this, &ChatDialog::slot_item_clicked);

    //连接发送和保存信息
    connect(ui->chat_page, &ChatPage::sig_append_send_chat_msg, this, &ChatDialog::slot_append_send_data_msg);

    //连接对端消息
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_text_chat_msg, this, &ChatDialog::slot_text_chat_msg);
}

bool ChatDialog::eventFilter(QObject* watched, QEvent* event){
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        HandleGlobalMousePress(mouseEvent);
    }
    return QDialog::eventFilter(watched, event);
}

void ChatDialog::HandleGlobalMousePress(QMouseEvent* event){
    // 实现点击位置的判断和处理逻辑
    // 先判断是否处于搜索模式，如果不处于搜索模式则直接返回
    if( _mode != ChatUIMode::SearchMode){
        return;
    }

    // 将鼠标点击位置转换为搜索列表坐标系中的位置
    QPoint posInSearchList = ui->search_list->mapFromGlobal(event->globalPos());
    qDebug()<<posInSearchList;
    qDebug()<<ui->search_list->rect().contains(posInSearchList);
    // 判断点击位置是否在聊天列表的范围内
    if (!ui->search_list->rect().contains(posInSearchList)) {
        // 如果不在聊天列表内，清空输入框
        ui->search_edit->clear();
        ShowSearch(false);
    }
}

void ChatDialog::AddLBGroup(StateWidget* wid){
    _lb_list.push_back(wid);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

//什么作用。。
void ChatDialog::ShowSearch(bool search){
    if(search){
        ui->con_user_list->hide();
        ui->chat_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    }
    else if(_state == ChatUIMode::ChatMode){
        ui->con_user_list->hide();
        ui->chat_user_list->show();
        ui->search_list->hide();
        _mode = ChatUIMode::ChatMode;
    }else if(_state == ChatUIMode::ContactMode){
        ui->con_user_list->show();
        ui->chat_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ContactMode;
    }
}

void ChatDialog::SetSelectChatItem(int uid)
{
    //什么意思，为什么要return
    if(ui->chat_user_list->count() <= 0){
        return;
    }

    if(uid == 0){
        ui->chat_user_list->setCurrentRow(0);
        QListWidgetItem *firstItem = ui->chat_user_list->item(0);
        if(!firstItem){
            return;
        }

        //转为widget
        QWidget *widget = ui->chat_user_list->itemWidget(firstItem);
        if(!widget){
            return;
        }

        auto con_item = qobject_cast<ChatuserWid*>(widget);
        if(!con_item){
            return;
        }

        _cur_chat_uid = con_item->GetInfo()->_uid;

        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        qDebug() << "uid " <<uid<< " not found, set curent row 0";
        ui->chat_user_list->setCurrentRow(0);
        return;
    }
    ui->chat_user_list->setCurrentItem(find_iter.value());

    _cur_chat_uid = uid;
}

void ChatDialog::SetSelectChatPage(int uid)
{
    if(ui->chat_user_list->count() <= 0){
        return;
    }

    if(uid == 0){
        auto item = ui->chat_user_list->item(0);
        //转为widget
        QWidget* widget = ui->chat_user_list->itemWidget(item);
        if (!widget) {
            return;
        }

        auto con_item = qobject_cast<ChatuserWid*>(widget);
        if (!con_item) {
            return;
        }

        //设置信息
        auto user_info = con_item->GetInfo();
        ui->chat_page->SetUserInfo(user_info);
        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        return;
    }

    //转为widget
    QWidget *widget = ui->chat_user_list->itemWidget(find_iter.value());
    if(!widget){
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == CHAT_USER_ITEM){
        auto con_item = qobject_cast<ChatuserWid*>(customItem);
        if(!con_item){
            return;
        }

        //设置信息
        auto user_info = con_item->GetInfo();
        ui->chat_page->SetUserInfo(user_info);

        return;
    }
}


void ChatDialog::addChatUserList()
{
    //先按照好友列表加载聊天记录，等以后客户端实现聊天记录数据库之后再按照最后信息排序
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if(find_iter != _chat_items_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatuserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid, item);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateChatLoadedCount();
    }

    // 创建QListWidgetItem，并设置自定义的widget
    // for(int i = 0; i < 13; i++){
    //     int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    //     int str_i = randomValue%strs.size();
    //     int head_i = randomValue%heads.size();
    //     int name_i = randomValue%names.size();
    //     auto *chat_user_wid = new ChatuserWid();
    //     chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
    //     QListWidgetItem *item = new QListWidgetItem;
    //     //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    //     item->setSizeHint(chat_user_wid->sizeHint());
    //     ui->chat_user_list->addItem(item);
    //     ui->chat_user_list->setItemWidget(item, chat_user_wid);
    // }
}

void ChatDialog::loadMoreChatUser() {
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if(find_iter != _chat_items_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatuserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid, item);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateChatLoadedCount();
    }
}

void ChatDialog::loadMoreConUser()
{
    auto friend_list = UserMgr::GetInstance()->GetConListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto *chat_user_wid = new ConUserItem();
            chat_user_wid->SetInfo(friend_ele->_uid,friend_ele->_name,
                                   friend_ele->_icon);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->con_user_list->addItem(item);
            ui->con_user_list->setItemWidget(item, chat_user_wid);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateContactLoadedCount();
    }
}

void ChatDialog::UpdateChatMsg(std::vector<std::shared_ptr<TextChatData>> msgdata)
{
    for(auto & msg : msgdata){
        if(msg->_from_uid != _cur_chat_uid){
            break;
        }

        ui->chat_page->AppendChatMsg(msg);
    }
}

void ChatDialog::slot_side_chat()
{
    qDebug()<< "receive side chat clicked";
    ClearLabelState(ui->side_chat_lb);
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    _state = ChatUIMode::ChatMode;
    ShowSearch(false);
}

void ChatDialog::slot_side_contact(){
    qDebug()<< "receive side contact clicked";
    ClearLabelState(ui->side_contact_lb);
    //ui->stackedWidget->setCurrentWidget(ui->contact_page);
    _state = ChatUIMode::ContactMode;
    ShowSearch(false);
}

//除了自己都进行清除状态
void ChatDialog::ClearLabelState(StateWidget* lb){
    for(auto it : _lb_list){
        if(lb == it){
            continue;
        }
        it->ClearState();
    }
}

void ChatDialog::slot_text_changed(const QString& str){
    if(!str.isEmpty()){
        ShowSearch(true);
    }
}

void ChatDialog::slot_switch_apply_page()
{
    ui->stackedWidget->setCurrentWidget(ui->apply_friend_page);
}

void ChatDialog::slot_loading_chat_user()
{
    if(_b_loading){
        return;
    }

    _b_loading = true;
    //LoadingDlg *loadingDialog = new LoadingDlg(this);
    //loadingDialog->setModal(true);
    //loadingDialog->show();
    qDebug() << "add new data to list.....";
    loadMoreChatUser();
    // 加载完成后关闭对话框
    //loadingDialog->deleteLater();

    _b_loading = false;
}

void ChatDialog::slot_loading_contact_user()
{
    qDebug() << "slot loading contact user";
    if(_b_loading){
        return;
    }

    _b_loading = true;
    //LoadingDlg *loadingDialog = new LoadingDlg(this);
    //loadingDialog->setModal(true);
    //loadingDialog->show();
    qDebug() << "add new data to list.....";
    loadMoreConUser();
    // 加载完成后关闭对话框
    //loadingDialog->deleteLater();

    _b_loading = false;
}

void ChatDialog::slot_friend_apply(std::shared_ptr<AddFriendApply> apply){
    qDebug() << "receive apply friend slot, applyuid is " << apply->_from_uid << " name is "
             << apply->_name << " desc is " << apply->_desc;

    bool b_apply = UserMgr::GetInstance()->AlreadyApply(apply->_from_uid);
    if(b_apply){
        return;
    }

    UserMgr::GetInstance()->AddApplyList(std::make_shared<ApplyInfo>(apply));
    ui->side_contact_lb->ShowRedPoint(true);
    ui->con_user_list->ShowRedPoint(true);
    ui->apply_friend_page->AddNewApply(apply);
}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "receive slot_auth_rsp uid is " << auth_rsp->_uid
             << " name is " << auth_rsp->_name << " nick is " << auth_rsp->_nick;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->_uid);
    if(bfriend){
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_rsp);
    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue % strs.size();
    int head_i = randomValue % heads.size();
    int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatuserWid();
    auto user_info = std::make_shared<UserInfo>(auth_rsp);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_rsp->_uid, item);;

}

void ChatDialog::slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info)
{
    qDebug() << "receive slot_add_auth__friend uid is " << auth_info->_uid
             << " name is " << auth_info->_name << " nick is " << auth_info->_nick;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_info->_uid);
    if(bfriend){
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_info);

    int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    int str_i = randomValue % strs.size();
    int head_i = randomValue % heads.size();
    int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatuserWid();
    auto user_info = std::make_shared<UserInfo>(auth_info);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_info->_uid, item);
}

void ChatDialog::slot_jump_chat_item(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "slot jump chat item " << Qt::endl;
    auto find_iter = _chat_items_added.find(si->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << si->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(si->_uid);
        //更新聊天界面信息
        SetSelectChatPage(si->_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatuserWid();
    auto user_info = std::make_shared<UserInfo>(si);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(si->_uid, item);

    ui->side_chat_lb->SetSelected(true);
    SetSelectChatItem(si->_uid);
    //更新聊天界面信息
    SetSelectChatPage(si->_uid);
    slot_side_chat();
}

void ChatDialog::slot_friend_info_page(std::shared_ptr<UserInfo> user_info)
{
    qDebug()<<"receive switch friend info page sig";
    _last_widget = ui->friend_info_page;
    ui->stackedWidget->setCurrentWidget(ui->friend_info_page);
    ui->friend_info_page->SetInfo(user_info);
}

void ChatDialog::slot_jump_chat_item_from_infopage(std::shared_ptr<UserInfo> user_info)
{
    qDebug() << "slot jump chat item " << Qt::endl;
    auto find_iter = _chat_items_added.find(user_info->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << user_info->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lb->SetSelected(true);
        SetSelectChatItem(user_info->_uid);
        //更新聊天界面信息
        SetSelectChatPage(user_info->_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatuserWid();
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(user_info->_uid, item);

    ui->side_chat_lb->SetSelected(true);
    SetSelectChatItem(user_info->_uid);
    //更新聊天界面信息
    SetSelectChatPage(user_info->_uid);
    slot_side_chat();
}

void ChatDialog::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = ui->chat_user_list->itemWidget(item); // 获取自定义widget对象
    if(!widget){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == ListItemType::INVALID_ITEM
        || itemType == ListItemType::GROUP_TIP_ITEM){
        qDebug()<< "slot invalid item clicked ";
        return;
    }


    if(itemType == ListItemType::CHAT_USER_ITEM){
        // 创建对话框，提示用户
        qDebug()<< "contact user item clicked ";

        auto chat_wid = qobject_cast<ChatuserWid*>(customItem);
        auto user_info = chat_wid->GetInfo();
        //跳转到聊天界面
        ui->chat_page->SetUserInfo(user_info);
        _cur_chat_uid = user_info->_uid;
        return;
    }
}

void ChatDialog::slot_append_send_data_msg(std::shared_ptr<TextChatData> msgdata) {
    if (_cur_chat_uid == 0) {
        return;
    }

    auto find_iter = _chat_items_added.find(_cur_chat_uid);
    if (find_iter == _chat_items_added.end()) {
        return;
    }

    //转为widget
    QWidget* widget = ui->chat_user_list->itemWidget(find_iter.value());
    if (!widget) {
        return;
    }

    //判断转化为自定义的widget
    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase* customItem = qobject_cast<ListItemBase*>(widget);
    if (!customItem) {
        qDebug() << "qobject_cast<ListItemBase*>(widget) is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if (itemType == CHAT_USER_ITEM) {
        auto con_item = qobject_cast<ChatuserWid*>(customItem);
        if (!con_item) {
            return;
        }

        //设置信息
        auto user_info = con_item->GetInfo();
        user_info->_chat_msgs.push_back(msgdata);
        std::vector<std::shared_ptr<TextChatData>> msg_vec;
        msg_vec.push_back(msgdata);
        UserMgr::GetInstance()->AppendFriendChatMsg(_cur_chat_uid,msg_vec);
        return;
    }
}

void ChatDialog::slot_text_chat_msg(std::shared_ptr<TextChatMsg> msg)
{
    auto find_iter = _chat_items_added.find(msg->_from_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "set chat item msg, uid is " << msg->_from_uid;
        QWidget *widget = ui->chat_user_list->itemWidget(find_iter.value());
        auto chat_wid = qobject_cast<ChatuserWid*>(widget);
        if(!chat_wid){
            return;
        }
        chat_wid->updateLastMsg(msg->_chat_msgs);
        //更新当前聊天页面记录
        UpdateChatMsg(msg->_chat_msgs);
        UserMgr::GetInstance()->AppendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatuserWid();
    //查询好友信息
    auto fi_ptr = UserMgr::GetInstance()->GetFriendById(msg->_from_uid);
    chat_user_wid->SetInfo(fi_ptr);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    chat_user_wid->updateLastMsg(msg->_chat_msgs);
    UserMgr::GetInstance()->AppendFriendChatMsg(msg->_from_uid,msg->_chat_msgs);
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(msg->_from_uid, item);

}
