/********************************************************************************
** Form generated from reading UI file 'resetdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESETDIALOG_H
#define UI_RESETDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ResetDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *err_tip;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *user_edit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *email_edit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *code_edit;
    QPushButton *get_code_btn;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QLineEdit *pass_edit;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *confirm_edit;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *ensure_btn;
    QPushButton *return_btn;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *ResetDialog)
    {
        if (ResetDialog->objectName().isEmpty())
            ResetDialog->setObjectName("ResetDialog");
        ResetDialog->resize(300, 500);
        ResetDialog->setMinimumSize(QSize(300, 500));
        ResetDialog->setMaximumSize(QSize(300, 500));
        verticalLayout_2 = new QVBoxLayout(ResetDialog);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        err_tip = new QLabel(ResetDialog);
        err_tip->setObjectName("err_tip");
        err_tip->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(err_tip);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(ResetDialog);
        label->setObjectName("label");
        label->setMinimumSize(QSize(0, 25));
        label->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout->addWidget(label);

        user_edit = new QLineEdit(ResetDialog);
        user_edit->setObjectName("user_edit");
        user_edit->setMinimumSize(QSize(0, 25));
        user_edit->setMaximumSize(QSize(213, 16777215));

        horizontalLayout->addWidget(user_edit);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 5);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_4 = new QLabel(ResetDialog);
        label_4->setObjectName("label_4");
        label_4->setMinimumSize(QSize(0, 25));
        label_4->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_2->addWidget(label_4);

        email_edit = new QLineEdit(ResetDialog);
        email_edit->setObjectName("email_edit");
        email_edit->setMinimumSize(QSize(0, 25));
        email_edit->setMaximumSize(QSize(213, 16777215));

        horizontalLayout_2->addWidget(email_edit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_3 = new QLabel(ResetDialog);
        label_3->setObjectName("label_3");
        label_3->setMinimumSize(QSize(0, 25));
        label_3->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_3->addWidget(label_3);

        code_edit = new QLineEdit(ResetDialog);
        code_edit->setObjectName("code_edit");
        code_edit->setMinimumSize(QSize(0, 25));
        code_edit->setMaximumSize(QSize(147, 16777215));

        horizontalLayout_3->addWidget(code_edit);

        get_code_btn = new QPushButton(ResetDialog);
        get_code_btn->setObjectName("get_code_btn");
        get_code_btn->setMinimumSize(QSize(0, 25));
        get_code_btn->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_3->addWidget(get_code_btn);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_2 = new QLabel(ResetDialog);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(0, 25));
        label_2->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_4->addWidget(label_2);

        pass_edit = new QLineEdit(ResetDialog);
        pass_edit->setObjectName("pass_edit");
        pass_edit->setMinimumSize(QSize(0, 25));
        pass_edit->setMaximumSize(QSize(213, 16777215));

        horizontalLayout_4->addWidget(pass_edit);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        label_6 = new QLabel(ResetDialog);
        label_6->setObjectName("label_6");
        label_6->setMinimumSize(QSize(0, 25));
        label_6->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_6->addWidget(label_6);

        confirm_edit = new QLineEdit(ResetDialog);
        confirm_edit->setObjectName("confirm_edit");
        confirm_edit->setMinimumSize(QSize(0, 25));
        confirm_edit->setMaximumSize(QSize(213, 16777215));

        horizontalLayout_6->addWidget(confirm_edit);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        ensure_btn = new QPushButton(ResetDialog);
        ensure_btn->setObjectName("ensure_btn");
        ensure_btn->setMinimumSize(QSize(0, 25));

        horizontalLayout_5->addWidget(ensure_btn);

        return_btn = new QPushButton(ResetDialog);
        return_btn->setObjectName("return_btn");
        return_btn->setMinimumSize(QSize(0, 25));

        horizontalLayout_5->addWidget(return_btn);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ResetDialog);

        QMetaObject::connectSlotsByName(ResetDialog);
    } // setupUi

    void retranslateUi(QDialog *ResetDialog)
    {
        ResetDialog->setWindowTitle(QCoreApplication::translate("ResetDialog", "Dialog", nullptr));
        err_tip->setText(QString());
        label->setText(QCoreApplication::translate("ResetDialog", "\347\224\250\346\210\267\345\220\215:", nullptr));
        label_4->setText(QCoreApplication::translate("ResetDialog", "\351\202\256\347\256\261:", nullptr));
        label_3->setText(QCoreApplication::translate("ResetDialog", "\351\252\214\350\257\201\347\240\201:", nullptr));
        get_code_btn->setText(QCoreApplication::translate("ResetDialog", "\350\216\267\345\217\226", nullptr));
        label_2->setText(QCoreApplication::translate("ResetDialog", "\346\226\260\345\257\206\347\240\201:", nullptr));
        label_6->setText(QCoreApplication::translate("ResetDialog", "\347\241\256\350\256\244\345\257\206\347\240\201:", nullptr));
        ensure_btn->setText(QCoreApplication::translate("ResetDialog", "\347\241\256\350\256\244", nullptr));
        return_btn->setText(QCoreApplication::translate("ResetDialog", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResetDialog: public Ui_ResetDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESETDIALOG_H
