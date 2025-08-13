#include "grouptipitem.h"
#include "ui_grouptipitem.h"

GroupTipItem::GroupTipItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::GroupTipItem)
{
    ui->setupUi(this);
    this->SetItemType(ListItemType::GROUP_TIP_ITEM);
}

GroupTipItem::~GroupTipItem()
{
    delete ui;
}

QSize GroupTipItem::SizeHint(){
    return QSize(250, 35);
}

void GroupTipItem::SetGroupTip(QString str){
    ui->label->setText(str);
}
