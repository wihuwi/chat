#include "adduseritem.h"
#include "ui_adduseritem.h"
#include <QEvent>

AddUserItem::AddUserItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::AddUserItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::ADD_USER_TIP_ITEM);
    
    // 启用鼠标追踪
    this->setMouseTracking(true);
    
    // 设置初始背景色
    this->setStyleSheet("background-color: rgb(247,247,247); border: none;");
}

AddUserItem::~AddUserItem()
{
    delete ui;
}

