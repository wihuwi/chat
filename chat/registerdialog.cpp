#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
    , _return_count(5)
{
    ui->setupUi(this);
    connect(ui->cancle_btn, &QPushButton::clicked, this, &RegisterDialog::CancleClicked);
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

    //ui->err_tip->setProperty("state", "normal");
    //repolish(ui->err_tip);
    ui->err_tip->clear();
    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this](){
        CheckUserValid();
    });
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        CheckEmailValid();
    });
    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this](){
        CheckPassValid();
    });
    connect(ui->varify_edit, &QLineEdit::editingFinished, this, [this](){
        CheckConfirmValid();
    });
    connect(ui->code_edit, &QLineEdit::editingFinished, this, [this](){
        CheckCodeValid();
    });

    InitHttpHandler();

    //密码 & 确认密码 & 密码可见
    //不设置objectname呢
    ui->pass_visible->SetState("visible", "visible_hover", "visible_hover", "unvisible", "unvisible_hover", "unvisible_hover");
    ui->confirm_visible->SetState("visible", "visible_hover", "visible_hover", "unvisible", "unvisible_hover", "unvisible_hover");
    ui->pass_edit->setEchoMode(QLineEdit::Normal);
    ui->varify_edit->setEchoMode(QLineEdit::Normal);
    connect(ui->pass_visible, &ClickedLabel::clicked, this, [this](){
        if(ui->pass_visible->GetState() == ClickLbState::Normal){
            this->ui->pass_edit->setEchoMode(QLineEdit::Normal);
        }
        else{
            this->ui->pass_edit->setEchoMode(QLineEdit::Password);
        }
    });
    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this](){
        if(ui->confirm_visible->GetState() == ClickLbState::Normal){
            this->ui->varify_edit->setEchoMode(QLineEdit::Normal);
        }
        else{
            this->ui->varify_edit->setEchoMode(QLineEdit::Password);
        }
    });

    //return page init
    _return_timer = new QTimer(this);
    _return_timer->stop();
    connect(_return_timer, &QTimer::timeout, this, [this](){
        if(_return_count == 0){
            _return_timer->stop();
            emit CancleClicked();
            return;
        }
        _return_count--;
        auto str = QString("注册成功，%1 s后返回登录").arg(_return_count);
        ui->returnLab->setText(str);
    });
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_getcode_btn_clicked()
{
    auto email = ui->email_edit->text();
    QRegularExpression regular = QRegularExpression(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool is_ok = regular.match(email).hasMatch();

    if(is_ok){
        QJsonObject jsonObj;
        jsonObj["email"] = ui->email_edit->text();
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_varifycode"), jsonObj,
                                            ReqId::ID_GET_VARIFY_CODE, Modules::REGISTERMOD);
    }else{
        showTips("邮箱错误", is_ok);
    }
}

void RegisterDialog::on_sure_btn_clicked(){
    if(ui->user_edit->text() == ""){
        showTips("用户名不能为空", false);
        return;
    }
    if(ui->email_edit->text() == ""){
        showTips("邮箱不能为空", false);
        return;
    }
    if(ui->pass_edit->text() == ""){
        showTips("密码不能为空", false);
        return;
    }
    if(ui->code_edit->text() == ""){
        showTips("验证码不能为空", false);
        return;
    }
    if(ui->pass_edit->text() != ui->varify_edit->text()){
        showTips("两次密码不一致", false);
        return;
    }

    QJsonObject jsonObj;
    jsonObj["user"] = ui->user_edit->text();
    jsonObj["email"] = ui->email_edit->text();
    jsonObj["passwd"] = ui->pass_edit->text();
    jsonObj["confirm"] = ui->varify_edit->text();
    jsonObj["varifycode"] = ui->code_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/user_register"),
                                        jsonObj, ReqId::ID_REG_USER, Modules::REGISTERMOD);

}

void RegisterDialog::showTips(const QString& str, bool is_ok){
    if(is_ok){
        ui->err_tip->setProperty("state", "normal");
    }
    else{
        ui->err_tip->setProperty("state", "err");
    }
    ui->err_tip->setText(str);
    qDebug()<<str;
    repolish(ui->err_tip);
}

void RegisterDialog::slot_reg_mod_finish(ReqId req_id, QString res, ErrorCodes err){
    if(err != ErrorCodes::SUCCESS){
        showTips(tr("网络请求错误"), false);
        return;
    }

    //解析Json字符串，将 res 转化为 QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()){
        showTips(tr("Json解析错误"), false);
        return;
    }

    //解析错误
    if(!jsonDoc.isObject()){
        showTips(tr("Json解析错误"), false);
        return;
    }

    _handler[req_id](jsonDoc.object());
}

void RegisterDialog::InitHttpHandler(){
    _handler.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject& jsonObj){
        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            std::cout<<"error code" <<err;
            showTips("参数错误", false);
            return;
        }

        auto email = jsonObj["email"];
        showTips("验证码已发送到邮箱", true);
        qDebug() << "email is" << email;
        return;
    });

    _handler.insert(ReqId::ID_REG_USER, [this](const QJsonObject& jsonObj){
        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            std::cout<<"error code" <<err;
            showTips("参数错误", false);
            return;
        }
        auto email = jsonObj["email"];
        showTips("注册成功", true);
        qDebug()<<"email is: "<<email;
        RegisterSuccess();
        return;
    });
}

void RegisterDialog::keyPressEvent(QKeyEvent* event){
    if(event->modifiers() == Qt::CTRL && event->key() == Qt::Key_V){
        ui->email_edit->setText("2697595253@qq.com");
    }
}

void RegisterDialog::AddTipErr(TipErr err, QString str){
    tips_err[err] = str;
    showTips(str, false);
}

void RegisterDialog::DelTipErr(TipErr err){
    tips_err.remove(err);
    if(tips_err.empty()){
        showTips("", true);
        return;
    }
    showTips(tips_err.first(), false);
}

bool RegisterDialog::CheckUserValid(){
    if(ui->user_edit->text() == ""){
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::CheckEmailValid(){
    //验证邮箱的地址正则表达式
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::CheckPassValid(){
    auto pass = ui->pass_edit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool RegisterDialog::CheckConfirmValid(){
    if(ui->pass_edit->text() != ui->varify_edit->text()){
        AddTipErr(TipErr::TIP_PWD_CONFIRM, tr("两次密码不一致"));
        return false;
    }
    DelTipErr(TipErr::TIP_PWD_CONFIRM);
    return true;
}

bool RegisterDialog::CheckCodeValid(){
    auto pass = ui->varify_edit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VARIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_VARIFY_ERR);
    return true;
}

void RegisterDialog::RegisterSuccess(){

    ui->stackedWidget->setCurrentWidget(ui->page_2);
    _return_timer->start(1000);
}

void RegisterDialog::on_returnBtn_clicked()
{
    _return_timer->stop();
    emit CancleClicked();
}

