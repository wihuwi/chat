#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->pass_edit->setEchoMode(QLineEdit::Password);

    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::RegisterClicked);

    //init forget
    ui->forgetLab->SetState("normal", "normal_hover","select", "select_hover");
    connect(ui->forgetLab, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);

    //连接输入提示错误
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        CheckEmailValid();
    });
    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this](){
        CheckPassValid();
    });
    //连接HttpMgr发送的信号 & 初始化处理函数
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_login_mod_finish, this, &LoginDialog::slot_login_mod_finish);
    InitHandler();

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::ShowTip(const QString& str, bool state){
    if(state){
        ui->err_tip->setProperty("state", "normal");
    }
    else{
        ui->err_tip->setProperty("state", "err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void LoginDialog::AddErr(TipErr err, const QString& str){
    _err_tips[err] = str;
    ShowTip(str, false);
}

void LoginDialog::DelErr(TipErr err){
    _err_tips.remove(err);
    if(_err_tips.empty()){
        ShowTip("", true);
        return;
    }
    ShowTip(_err_tips.first(), false);
}

bool LoginDialog::CheckEmailValid(){
    auto email = ui->email_edit->text();
    QRegularExpression expression(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = expression.match(email).hasMatch();
    if(!match){
        AddErr(TipErr::TIP_EMAIL_ERR, "邮箱不正确");
        return false;
    }
    DelErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::CheckPassValid(){
    auto pass = ui->pass_edit->text();
    if(pass.length() < 6 || pass.length() > 15){
        AddErr(TipErr::TIP_PWD_ERR, "密码长度6 ~ 15");
        return false;
    }

    DelErr(TipErr::TIP_PWD_ERR);
    return true;
}

void LoginDialog::InitHandler(){
    _handlers.insert(ReqId::ID_LOGIN_USER, [this](const QJsonObject& jsonObj){
        if(jsonObj["error"].toInt()){
            std::cout<<"handler occur error: "<<jsonObj["error"].toInt();
            ShowTip("处理错误", false);
            return;
        }

        ServerInfo server;
        server.Host = jsonObj["host"].toString();
        server.Port = jsonObj["port"].toString();
        server.Token = jsonObj["token"].toString();
        server.Uid = jsonObj["uid"].toInt();

        emit sig_connect_tcp(server);

        _uid = server.Uid;
        _token = server.Token;

        ShowTip("登陆成功", true);

    });
}

void LoginDialog::slot_forget_pwd(){
    qDebug()<<"slot_forget_pwd";
    emit SwitchReset();
}

void LoginDialog::on_log_btn_clicked()
{
    if(CheckEmailValid() && CheckPassValid()){
        QJsonObject jsonObj;
        jsonObj["email"] = ui->email_edit->text();
        jsonObj["passwd"] = ui->pass_edit->text();
        QString url = gate_url_prefix + "/user_login";
        HttpMgr::GetInstance()->PostHttpReq(url, jsonObj,
                                            ReqId::ID_LOGIN_USER, Modules::LOGINMOD);
    }
}

void LoginDialog::slot_login_mod_finish(ReqId reqId, QString data, ErrorCodes err){
    if(err != ErrorCodes::SUCCESS){
        std::cout<<"slot_login_mod_finish occur error: "<<err;
        ShowTip("网络请求错误", false);
        return;
    }
    qDebug()<<data;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    if(jsonDoc.isNull()){
        std::cout<<"QJsonDocument occur error: "<<err;
        ShowTip("Json 解析错误", false);
        return;
    }
    qDebug()<<jsonDoc;
    if(!jsonDoc.isObject()){
        std::cout<<"QJsonDocument object occur error: "<<err;
        ShowTip("Json 解析错误", false);
        return;
    }

    _handlers[reqId](jsonDoc.object());
}
