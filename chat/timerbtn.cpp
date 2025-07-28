#include "timerbtn.h"

TimerBtn::TimerBtn(QWidget* parent):QPushButton(parent), _count(3) {
    _timer = new QTimer(this);

    connect(_timer, &QTimer::timeout, this, [this](){
        _count--;
        if(_count < 0){
            _timer->stop();
            this->setEnabled(true);
            this->setText("获取");
            _count = 3;
            return;
        }
        this->setText(QString::number(_count));
    });
}

TimerBtn::~TimerBtn(){
    _timer->stop();
}

void TimerBtn::mouseReleaseEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        this->setEnabled(false);
        this->setText(QString::number(_count));
        _timer->start(1000);
        emit this->clicked();
    }

    QPushButton::mouseReleaseEvent(event);
}
