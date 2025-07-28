/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <clickedlabel.h>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *user_lab;
    QLineEdit *name_edit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *pass_lab;
    QLineEdit *pass_edit;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_5;
    ClickedLabel *forgetLab;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *log_btn;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *reg_btn;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(300, 500);
        LoginDialog->setMinimumSize(QSize(300, 500));
        LoginDialog->setMaximumSize(QSize(300, 500));
        verticalLayout_2 = new QVBoxLayout(LoginDialog);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        widget = new QWidget(LoginDialog);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(300, 200));
        widget->setSizeIncrement(QSize(300, 200));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_3 = new QLabel(widget);
        label_3->setObjectName("label_3");
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/icons/icon.ico")));

        verticalLayout_3->addWidget(label_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        verticalLayout->addWidget(widget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        user_lab = new QLabel(LoginDialog);
        user_lab->setObjectName("user_lab");

        horizontalLayout_2->addWidget(user_lab);

        name_edit = new QLineEdit(LoginDialog);
        name_edit->setObjectName("name_edit");

        horizontalLayout_2->addWidget(name_edit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        pass_lab = new QLabel(LoginDialog);
        pass_lab->setObjectName("pass_lab");

        horizontalLayout_3->addWidget(pass_lab);

        pass_edit = new QLineEdit(LoginDialog);
        pass_edit->setObjectName("pass_edit");

        horizontalLayout_3->addWidget(pass_edit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        forgetLab = new ClickedLabel(LoginDialog);
        forgetLab->setObjectName("forgetLab");

        horizontalLayout_5->addWidget(forgetLab);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        log_btn = new QPushButton(LoginDialog);
        log_btn->setObjectName("log_btn");

        horizontalLayout->addWidget(log_btn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        reg_btn = new QPushButton(LoginDialog);
        reg_btn->setObjectName("reg_btn");

        horizontalLayout_4->addWidget(reg_btn);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Dialog", nullptr));
        label_3->setText(QString());
        user_lab->setText(QCoreApplication::translate("LoginDialog", "\350\264\246\345\217\267\357\274\232", nullptr));
        pass_lab->setText(QCoreApplication::translate("LoginDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        forgetLab->setText(QCoreApplication::translate("LoginDialog", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
        log_btn->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        reg_btn->setText(QCoreApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
