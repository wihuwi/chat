#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::RegisterClicked);

    //init forget
    ui->forgetLab->SetState("normal", "normal_hover","select", "select_hover");
    connect(ui->forgetLab, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::slot_forget_pwd(){
    qDebug()<<"slot_forget_pwd";
    emit SwitchReset();
}
