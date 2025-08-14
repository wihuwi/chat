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
    , ui(new Ui::ChatDialog), _mode(ChatUIMode::ChatMode), _state(ChatUIMode::ChatMode), _b_loading(false)
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

    //搜索框
    connect(ui->search_edit, &QLineEdit::textChanged, this, &ChatDialog::slot_text_changed);

    //侧边栏
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

    //连接添加好友通知回包 -》
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_friend_apply, this, &ChatDialog::slot_friend_apply);
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
    }else{
        ui->con_user_list->show();
        ui->chat_user_list->hide();
        ui->search_list->hide();
        _mode = ChatUIMode::ContactMode;
    }
}


void ChatDialog::addChatUserList()
{
    // 创建QListWidgetItem，并设置自定义的widget
    for(int i = 0; i < 13; i++){
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();
        auto *chat_user_wid = new ChatuserWid();
        chat_user_wid->SetInfo(names[name_i], heads[head_i], strs[str_i]);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(chat_user_wid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chat_user_wid);
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
