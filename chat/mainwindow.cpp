#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口图标
    QIcon icon(":/icons/icon.ico");
    this->setWindowIcon(icon);
    _log_ = new LoginDialog(this);
    connect(_log_, &LoginDialog::RegisterClicked, this, &MainWindow::SwitchRegister);
    connect(_log_, &LoginDialog::SwitchReset, this, &MainWindow::SwitchReset);
    _log_->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_log_);
    _log_->show();

    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_switch_chatdlg, this, &MainWindow::SwitchChat);
    //TcpMgr::GetInstance()->sig_switch_chatdlg();
}

MainWindow::~MainWindow()
{
    delete ui;

    // if(_log_){
    //     delete _log_;
    //     _log_ = nullptr;
    // }
    // if(_reg_){
    //     delete _reg_;
    //     _reg_ = nullptr;
    // }
}

void MainWindow::SwitchRegister(){
    _reg_ = new RegisterDialog(this);
    _reg_->hide();
    _reg_->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    connect(_reg_, &RegisterDialog::CancleClicked, this, &MainWindow::SwitchLogin);
    setCentralWidget(_reg_);
    //_log_->hide();
    //不屑hide用影响吗
    _reg_->show();
}

void MainWindow::SwitchLogin(){
    _log_ = new LoginDialog(this);
    connect(_log_, &LoginDialog::RegisterClicked, this, &MainWindow::SwitchRegister);
    connect(_log_, &LoginDialog::SwitchReset, this, &MainWindow::SwitchReset);
    _log_->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_log_);
    //_reg_->hide();
    _log_->show();
}

void MainWindow::SwitchReset(){
    _reset_ = new ResetDialog(this);
    connect(_reset_, &ResetDialog::SwitchLogin, this, &MainWindow::SwitchLogin);
    _reset_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_reset_);
    //_log_->hide();
    _reset_->show();
}

void MainWindow::SwitchChat(){
    _chat_ = new ChatDialog(this);
    _chat_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    this->setCentralWidget(_chat_);
    _log_->hide();
    _chat_->show();
}
