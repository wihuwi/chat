#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include <QRegularExpression>
#include <functional>

#include "global.h"
#include "httpmgr.h"

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();

private:
    void ShowTip(QString str, bool state);
    void AddErr(TipErr err, QString str);
    void DelErr(TipErr err);
    bool CheckUserValid();
    bool CheckEmailValid();
    bool CheckCodeValid();
    bool CheckPassValid();
    bool CheckConfirmValid();
    void InitResetHandlers();

    Ui::ResetDialog *ui;
    QMap<TipErr, QString> err_tips;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handler;

private slots:
    void on_return_btn_clicked();

    void on_ensure_btn_clicked();

    void on_get_code_btn_clicked();

    void slot_reset_mod_finish(ReqId reqId, QString data, ErrorCodes err);

signals:
    void SwitchLogin();
};

#endif // RESETDIALOG_H
