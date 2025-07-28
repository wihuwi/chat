/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <clickedlabel.h>
#include <timerbtn.h>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QHBoxLayout *horizontalLayout_7;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QSpacerItem *verticalSpacer_4;
    QLabel *err_tip;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *user_lab;
    QLineEdit *user_edit;
    QHBoxLayout *horizontalLayout_5;
    QLabel *email_lab;
    QLineEdit *email_edit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *pass_lab;
    QLineEdit *pass_edit;
    ClickedLabel *pass_visible;
    QHBoxLayout *horizontalLayout_3;
    QLabel *varify_lab;
    QLineEdit *varify_edit;
    ClickedLabel *confirm_visible;
    QHBoxLayout *horizontalLayout_2;
    QLabel *varify_code_lab;
    QLineEdit *code_edit;
    TimerBtn *getcode_btn;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *sure_btn;
    QPushButton *cancle_btn;
    QSpacerItem *verticalSpacer_3;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_6;
    QLabel *returnLab;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *returnBtn;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_7;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(300, 500);
        RegisterDialog->setMinimumSize(QSize(300, 500));
        RegisterDialog->setMaximumSize(QSize(300, 500));
        horizontalLayout_7 = new QHBoxLayout(RegisterDialog);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        stackedWidget = new QStackedWidget(RegisterDialog);
        stackedWidget->setObjectName("stackedWidget");
        page = new QWidget();
        page->setObjectName("page");
        verticalLayout_4 = new QVBoxLayout(page);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        widget = new QWidget(page);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(0, 50));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label = new QLabel(widget);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(label);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_4);

        err_tip = new QLabel(widget);
        err_tip->setObjectName("err_tip");
        err_tip->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(err_tip);


        verticalLayout->addWidget(widget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        user_lab = new QLabel(page);
        user_lab->setObjectName("user_lab");
        user_lab->setMinimumSize(QSize(0, 25));
        user_lab->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(user_lab);

        user_edit = new QLineEdit(page);
        user_edit->setObjectName("user_edit");
        user_edit->setMinimumSize(QSize(0, 25));
        user_edit->setMaximumSize(QSize(206, 16777215));

        horizontalLayout->addWidget(user_edit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        email_lab = new QLabel(page);
        email_lab->setObjectName("email_lab");
        email_lab->setMinimumSize(QSize(0, 25));
        email_lab->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_5->addWidget(email_lab);

        email_edit = new QLineEdit(page);
        email_edit->setObjectName("email_edit");
        email_edit->setMinimumSize(QSize(0, 25));
        email_edit->setMaximumSize(QSize(206, 16777215));

        horizontalLayout_5->addWidget(email_edit);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        pass_lab = new QLabel(page);
        pass_lab->setObjectName("pass_lab");
        pass_lab->setMinimumSize(QSize(0, 25));
        pass_lab->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_4->addWidget(pass_lab);

        pass_edit = new QLineEdit(page);
        pass_edit->setObjectName("pass_edit");
        pass_edit->setMinimumSize(QSize(0, 25));
        pass_edit->setMaximumSize(QSize(180, 16777215));

        horizontalLayout_4->addWidget(pass_edit);

        pass_visible = new ClickedLabel(page);
        pass_visible->setObjectName("pass_visible");
        pass_visible->setMinimumSize(QSize(20, 20));
        pass_visible->setMaximumSize(QSize(20, 20));

        horizontalLayout_4->addWidget(pass_visible);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        varify_lab = new QLabel(page);
        varify_lab->setObjectName("varify_lab");
        varify_lab->setMinimumSize(QSize(0, 25));
        varify_lab->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(varify_lab);

        varify_edit = new QLineEdit(page);
        varify_edit->setObjectName("varify_edit");
        varify_edit->setMinimumSize(QSize(0, 25));
        varify_edit->setMaximumSize(QSize(180, 16777215));

        horizontalLayout_3->addWidget(varify_edit);

        confirm_visible = new ClickedLabel(page);
        confirm_visible->setObjectName("confirm_visible");
        confirm_visible->setMinimumSize(QSize(20, 20));
        confirm_visible->setMaximumSize(QSize(20, 20));

        horizontalLayout_3->addWidget(confirm_visible);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        varify_code_lab = new QLabel(page);
        varify_code_lab->setObjectName("varify_code_lab");
        varify_code_lab->setMinimumSize(QSize(0, 25));
        varify_code_lab->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_2->addWidget(varify_code_lab);

        code_edit = new QLineEdit(page);
        code_edit->setObjectName("code_edit");
        code_edit->setMinimumSize(QSize(0, 25));
        code_edit->setMaximumSize(QSize(140, 16777215));

        horizontalLayout_2->addWidget(code_edit);

        getcode_btn = new TimerBtn(page);
        getcode_btn->setObjectName("getcode_btn");
        getcode_btn->setMinimumSize(QSize(0, 25));
        getcode_btn->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_2->addWidget(getcode_btn);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        sure_btn = new QPushButton(page);
        sure_btn->setObjectName("sure_btn");

        horizontalLayout_6->addWidget(sure_btn);

        cancle_btn = new QPushButton(page);
        cancle_btn->setObjectName("cancle_btn");

        horizontalLayout_6->addWidget(cancle_btn);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        verticalLayout_4->addLayout(verticalLayout);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        verticalLayout_5 = new QVBoxLayout(page_2);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_6);

        returnLab = new QLabel(page_2);
        returnLab->setObjectName("returnLab");
        returnLab->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(returnLab);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_5);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);

        returnBtn = new QPushButton(page_2);
        returnBtn->setObjectName("returnBtn");

        horizontalLayout_8->addWidget(returnBtn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_8);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_7);


        verticalLayout_5->addLayout(verticalLayout_2);

        stackedWidget->addWidget(page_2);

        horizontalLayout_7->addWidget(stackedWidget);


        retranslateUi(RegisterDialog);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("RegisterDialog", "\346\263\250\345\206\214", nullptr));
        err_tip->setText(QCoreApplication::translate("RegisterDialog", "REG", nullptr));
        user_lab->setText(QCoreApplication::translate("RegisterDialog", "\347\224\250\346\210\267:", nullptr));
        email_lab->setText(QCoreApplication::translate("RegisterDialog", "\351\202\256\347\256\261:", nullptr));
        pass_lab->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\347\240\201:", nullptr));
        pass_visible->setText(QString());
        varify_lab->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244:", nullptr));
        confirm_visible->setText(QString());
        varify_code_lab->setText(QCoreApplication::translate("RegisterDialog", "\351\252\214\350\257\201\347\240\201:", nullptr));
        getcode_btn->setText(QCoreApplication::translate("RegisterDialog", "\350\216\267\345\217\226", nullptr));
        sure_btn->setText(QCoreApplication::translate("RegisterDialog", "\346\263\250\345\206\214", nullptr));
        cancle_btn->setText(QCoreApplication::translate("RegisterDialog", "\345\217\226\346\266\210", nullptr));
        returnLab->setText(QCoreApplication::translate("RegisterDialog", "\346\263\250\345\206\214\346\210\220\345\212\237\357\274\2145 s\345\220\216\350\277\224\345\233\236\347\231\273\345\275\225", nullptr));
        returnBtn->setText(QCoreApplication::translate("RegisterDialog", "\350\277\224\345\233\236\347\231\273\351\231\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
