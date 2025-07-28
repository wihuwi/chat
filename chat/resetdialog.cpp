#include "resetdialog.h"
#include "ui_resetdialog.h"

ResetDialog::ResetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResetDialog)
{
    ui->setupUi(this);

    //连接httpmgr完成reset处理的信号
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reset_mod_finish, this, &ResetDialog::slot_reset_mod_finish);
    //初始化处理函数
    InitResetHandlers();
    ui->err_tip->setObjectName("err_tip");
    //检查输入完符不符合
    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this](){
        CheckUserValid();
    });
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        CheckEmailValid();
    });
    connect(ui->code_edit, &QLineEdit::editingFinished, this, [this](){
        CheckCodeValid();
    });
    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this](){
        CheckPassValid();
    });
    connect(ui->confirm_edit, &QLineEdit::editingFinished, this, [this](){
        CheckConfirmValid();
    });
}

void ResetDialog::ShowTip(QString str, bool state){
    if(state == true){
        ui->err_tip->setProperty("state", "normal");
    }
    else{
        ui->err_tip->setProperty("state", "err");
    }
    qDebug()<<str;
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
    //需要设置在ui控件上
}

void ResetDialog::AddErr(TipErr err, QString str){
    err_tips[err] = str;
    ShowTip(str, false);
}

void ResetDialog::DelErr(TipErr err){
    err_tips.remove(err);
    if(err_tips.empty()){
        ShowTip("", true);
        return;
    }
    ShowTip(err_tips.first(), false);
}

bool ResetDialog::CheckUserValid(){
    if(ui->user_edit->text() == ""){
        AddErr(TipErr::TIP_USER_ERR, "用户名不为空");
        return false;
    }
    DelErr(TipErr::TIP_USER_ERR);
    return true;
}

bool ResetDialog::CheckEmailValid(){
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

bool ResetDialog::CheckCodeValid(){
    if(ui->code_edit->text() == ""){
        AddErr(TipErr::TIP_VARIFY_ERR, "验证码不为空");
        return false;
    }
    DelErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

bool ResetDialog::CheckPassValid(){
    auto pass = ui->pass_edit->text();
    if(pass.length() < 6 || pass.length() > 15){
        AddErr(TipErr::TIP_PWD_ERR, "密码长度6 ~ 15");
        return false;
    }
    QRegularExpression expression("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = expression.match(pass).hasMatch();
    if(!match){
        AddErr(TipErr::TIP_PWD_ERR, "密码包含字母，数字，!@#$%^&*");
        return false;
    }
    DelErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool ResetDialog::CheckConfirmValid(){
    if(ui->pass_edit->text() != ui->confirm_edit->text()){
        AddErr(TipErr::TIP_PWD_CONFIRM, "两次密码不一致");
        return false;
    }
    DelErr(TipErr::TIP_PWD_CONFIRM);
    return true;
}

void ResetDialog::InitResetHandlers(){
    _handler.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject& data){
        if(data["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"error code:" << data["error"].toInt();
            ShowTip("参数错误", false);
            return;
        }
        ShowTip("获取成功", true);
        auto email = data["验证码已发送到邮箱"];
        qDebug()<<"email: "<<email;
        return;
    });
    _handler.insert(ReqId::ID_RESET_PWD, [this](const QJsonObject& data){
        if(data["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"error code" << data["error"].toInt();
            ShowTip("参数错误", false);
            return;
        }
        auto email = data["email"].toString();
        ShowTip(tr("重置成功,点击返回进行登录"), true);
        qDebug()<< "email is " << email ;
        qDebug()<< "user uuid is " <<  data["uuid"].toString();
    });
}

ResetDialog::~ResetDialog()
{
    delete ui;
}

void ResetDialog::on_return_btn_clicked()
{
    emit SwitchLogin();
}


void ResetDialog::on_ensure_btn_clicked()
{
    if(CheckUserValid() && CheckEmailValid() && CheckCodeValid() && CheckPassValid() && CheckConfirmValid()){
        QJsonObject jsonObj;
        jsonObj["user"] = ui->user_edit->text();
        jsonObj["email"] = ui->email_edit->text();
        jsonObj["passwd"] = ui->pass_edit->text();
        jsonObj["varifycode"] = ui->code_edit->text();
        HttpMgr::GetInstance()->PostHttpReq(gate_url_prefix + "/reset_pwd", jsonObj,
                                            ReqId::ID_RESET_PWD, Modules::RESETMOD);
    }
}


void ResetDialog::on_get_code_btn_clicked()
{
    if(CheckEmailValid()){
        QJsonObject jsonObj;
        jsonObj["email"] = ui->email_edit->text();
        HttpMgr::GetInstance()->PostHttpReq(gate_url_prefix + "/get_varifycode", jsonObj,
                                            ReqId::ID_GET_VARIFY_CODE, Modules::RESETMOD);
    }
}

void ResetDialog::slot_reset_mod_finish(ReqId reqId, QString data, ErrorCodes err){
    if(err != ErrorCodes::SUCCESS){
        ShowTip(tr("网络请求错误"), false);
        return;
    }

    //解析Json字符串，将 res 转化为 QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    if(jsonDoc.isNull()){
        ShowTip(tr("Json解析错误"), false);
        return;
    }

    //解析错误
    if(!jsonDoc.isObject()){
        ShowTip(tr("Json解析错误"), false);
        return;
    }
    _handler[reqId](jsonDoc.object());
}
