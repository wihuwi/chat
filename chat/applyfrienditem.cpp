#include "applyfrienditem.h"
#include "ui_applyfrienditem.h"

ApplyFriendItem::ApplyFriendItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ApplyFriendItem),_added(false)
{
    ui->setupUi(this);
    SetItemType(ListItemType::APPLY_FRIEND_ITEM);
    ui->addBtn->SetState("normal","hover", "press");
    ui->addBtn->hide();
    connect(ui->addBtn, &ClickedBtn::clicked,  [this](){
        emit this->sig_auth_friend(_apply_info);
    });
}

ApplyFriendItem::~ApplyFriendItem()
{
    delete ui;
}

void ApplyFriendItem::ShowAddBtn(bool show){
    if(show){
        ui->addBtn->show();
        ui->already_add_lb->hide();
        _added = false;
    }else{
        ui->addBtn->hide();
        ui->already_add_lb->show();
        _added = true;
    }
}

void ApplyFriendItem::SetInfo(std::shared_ptr<ApplyInfo> info){
    _apply_info = info;

    QPixmap pix = QPixmap(info->_icon);
    ui->icon_lb->setPixmap(pix.scaled(ui->icon_lb->sizeHint(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->user_name_lb->setText(info->_name);
    ui->user_chat_lb->setText(info->_desc);
}

int ApplyFriendItem::GetUid()
{
    return _apply_info->_uid;
}
