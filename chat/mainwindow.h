#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include "logindialog.h"
#include "resetdialog.h"
#include "registerdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:


public slots:
    void SwitchRegister();
    void SwitchLogin();
    void SwitchReset();

private:
    Ui::MainWindow *ui;
    LoginDialog *_log_;
    RegisterDialog *_reg_;
    ResetDialog* _reset_;
};
#endif // MAINWINDOW_H
