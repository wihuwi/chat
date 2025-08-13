#ifndef FINDSUCCESSDIALOG_H
#define FINDSUCCESSDIALOG_H

#include <QDialog>
#include "userdata.h"

namespace Ui {
class FindSuccessDialog;
}

class FindSuccessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindSuccessDialog(QWidget *parent = nullptr);
    ~FindSuccessDialog();
    void SetSearchInfo(std::shared_ptr<SearchInfo> si);

private:
    Ui::FindSuccessDialog *ui;
    std::shared_ptr<SearchInfo> _si;
    QWidget* _parent;

private slots:
    void on_add_friend_btn_clicked();
};

#endif // FINDSUCCESSDIALOG_H
