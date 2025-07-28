#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QRegularExpression>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTimer>
#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

    void showTips(const QString&, bool is_ok);

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    void AddTipErr(TipErr, QString);
    void DelTipErr(TipErr);
    bool CheckUserValid();
    bool CheckEmailValid();
    bool CheckPassValid();
    bool CheckConfirmValid();
    bool CheckCodeValid();
    void RegisterSuccess();

    Ui::RegisterDialog *ui;
    void InitHttpHandler();
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handler;
    QMap<TipErr, QString> tips_err;
    QTimer* _return_timer;
    int _return_count;

signals:
    void CancleClicked();
private slots:
    void on_getcode_btn_clicked();
    void on_sure_btn_clicked();
    void slot_reg_mod_finish(ReqId, QString, ErrorCodes);
    void on_returnBtn_clicked();
};

#endif // REGISTERDIALOG_H
