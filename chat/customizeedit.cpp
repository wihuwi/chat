#include "customizeedit.h"
#include <QAction>

CustomizeEdit::CustomizeEdit(QWidget* parent):QLineEdit(parent), _max_len(21) {
    connect(this, &QLineEdit::textChanged, this, &CustomizeEdit::limitTextLength);
}

CustomizeEdit::~CustomizeEdit(){

}

void CustomizeEdit::SetMaxLen(int maxlen){
    _max_len = maxlen;
}

void CustomizeEdit::focusOutEvent(QFocusEvent* event){
    QLineEdit::focusOutEvent(event);
    emit sig_focus_out();
    //如果发信号错误 ，可能时Q_OBJECT没加上，加上需要重新qmake即可
}

void CustomizeEdit::limitTextLength(const QString& text){
    if(_max_len <= 0){
        return;
    }

    QByteArray bytes = text.toUtf8();

    if(bytes.length() > _max_len){
        bytes = bytes.left(_max_len);
        this->setText(QString::fromUtf8(bytes));
    }
}
