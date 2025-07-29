#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <functional>
#include <QRegularExpression>
#include <QJsonObject>
#include <QJsonDocument>

#include "global.h"
#include "httpmgr.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    void ShowTip(const QString& str, bool state);
    void AddErr(TipErr err, const QString& str);
    void DelErr(TipErr err);
    bool CheckEmailValid();
    bool CheckPassValid();
    void InitHandler();

    Ui::LoginDialog *ui;
    QMap<TipErr, QString> _err_tips;
    QMap<ReqId, std::function<void(const QJsonObject& obj)>> _handlers;
    int _uid;
    QString _token;

signals:
    void RegisterClicked();
    void SwitchReset();
    void sig_connect_tcp(const ServerInfo&);

private slots:
    void slot_forget_pwd();
    void on_log_btn_clicked();
    void slot_login_mod_finish(ReqId reqId, QString data, ErrorCodes err);
    void slot_con_success(bool success);
    void slot_login_failed(int err);
};

#endif // LOGINDIALOG_H
