#include "findsuccessdialog.h"
#include "ui_findsuccessdialog.h"
#include "applyfriend.h"
#include "usermgr.h"
#include <QDir>

FindSuccessDialog::FindSuccessDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindSuccessDialog)
    , _parent(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("Add");
    //setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    QString path = QDir::fromNativeSeparators(QCoreApplication::applicationDirPath() + QDir::separator() + "static" + QDir::separator() + "head_1.jpg");
    qDebug()<<path;
    QPixmap pixmap(path);
    QPixmap scaledPixmap = pixmap.scaled(ui->head_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_lb->setPixmap(scaledPixmap);
    //插入的要是新的sacledPixmap，不能pixmap

    ui->add_friend_btn->SetState("normal", "hover", "press");
    this->setModal(false);
}

FindSuccessDialog::~FindSuccessDialog()
{
    qDebug()<<"FindSuccessDialog destruct";
    delete ui;
}

void FindSuccessDialog::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    ui->name_lb->setText(si->_name);
    _si = si;
}

void FindSuccessDialog::on_add_friend_btn_clicked()
{
    this->hide();
    auto applyFriend = new ApplyFriend(_parent);
    applyFriend->SetSearchInfo(_si);
    applyFriend->setModal(true);
    applyFriend->SetPlaceHolder(_si->_name, UserMgr::GetInstance()->GetName());
    applyFriend->show();
}
