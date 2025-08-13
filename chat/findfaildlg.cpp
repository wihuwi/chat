#include "findfaildlg.h"
#include "ui_findfaildlg.h"

FindFailDlg::FindFailDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindFailDlg)
{
    ui->setupUi(this);
    this->setWindowTitle("Add");
    this->setModal(true);
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    ui->fail_sure_btn->SetState("normal", "hover", "press");
    this->setObjectName("FindFailDld");
}

FindFailDlg::~FindFailDlg()
{
    delete ui;
}

void FindFailDlg::on_fail_sure_btn_clicked()
{
    ui->fail_sure_btn->hide();
}

