#include "conuseritem.h"
#include "ui_conuseritem.h"

ConUserItem::ConUserItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ConUserItem)
{
    ui->setupUi(this);
    this->SetItemType(ListItemType::CONTACT_USER_ITEM);
    ui->red_point->raise();
    this->SetRedPoint(false);
}

ConUserItem::~ConUserItem()
{
    delete ui;
}

QSize ConUserItem::sizeHint() const
{
    return QSize(250, 50); // 返回自定义的尺寸
}

void ConUserItem::SetInfo(std::shared_ptr<AuthInfo> info){
    ui->user_name_lb->setText(info->_name);

    _info = std::make_shared<UserInfo>(info);

    QPixmap pixmap = QPixmap(info->_icon);

    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);


}

void ConUserItem::SetInfo(std::shared_ptr<AuthRsp> info){
    ui->user_name_lb->setText(info->_name);

    _info = std::make_shared<UserInfo>(info);

    QPixmap pixmap = QPixmap(info->_icon);
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);
}

void ConUserItem::SetInfo(int uid, QString name, QString icon){
    ui->user_name_lb->setText(name);

    _info = std::make_shared<UserInfo>(uid, name, icon);

    QPixmap pixmap = QPixmap(icon);
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);
}

void ConUserItem::SetRedPoint(bool redPoint){
    if(redPoint){
        ui->red_point->show();
    }else{
        ui->red_point->hide();
    }
}

std::shared_ptr<UserInfo> ConUserItem::GetInfo()
{
    return _info;
}
