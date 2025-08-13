#include "applyfriendlist.h"
#include <QScrollBar>
#include <QEvent>
#include <QWheelEvent>

ApplyFriendList::ApplyFriendList(QWidget* parent):QListWidget(parent) {
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->viewport()->installEventFilter(this);
}

bool ApplyFriendList::eventFilter(QObject* obj, QEvent* event){
    if(obj == this->viewport()){
        if(event->type() == QEvent::Enter){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }else if(event->type() == QEvent::Leave){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    if(event->type() == QEvent::MouseButtonPress && obj == this->viewport()){
        emit sig_show_search(false);
    }

    if(obj == this->viewport() && event->type() == QEvent::Wheel){
        auto wheelEvent = static_cast<QWheelEvent*>(event);
        int numSteps = wheelEvent->angleDelta().y() / 8 / 15;
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        return true;
    }
    return QListWidget::eventFilter(obj, event);
}
