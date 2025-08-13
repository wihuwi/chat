#include "bubbleframe.h"

BubbleFrame::BubbleFrame(QWidget* parent, int margin, ChatRole chatrole):QFrame(parent),
    _chat_role(chatrole), _margin(margin) {
    _h_layout = new QHBoxLayout(this);

    if(_chat_role == ChatRole::self){
        _h_layout->setContentsMargins(_margin, _margin, _margin + WIDTH_SANJIAO, _margin);
    }
    else{
        _h_layout->setContentsMargins(_margin + WIDTH_SANJIAO, _margin, _margin, _margin);
    }
    this->setLayout(_h_layout);
}

void BubbleFrame::paintEvent(QPaintEvent* event){
    QPainter painter = QPainter(this);
    painter.setPen(Qt::NoPen);

    if(_chat_role == ChatRole::self){
        painter.setBrush(Qt::green);
        QRect rect = QRect(0, 0, this->width() - WIDTH_SANJIAO, this->height());
        painter.drawRoundedRect(rect, 5, 5);

        QPointF points[] = {
            QPointF(rect.x() + rect.width(), 12),
            QPointF(rect.x() + rect.width(), 12 + WIDTH_SANJIAO),
            QPointF(rect.x() + rect.width() + WIDTH_SANJIAO / 2, 12 + WIDTH_SANJIAO / 2)
        };
        painter.drawPolygon(points, 3);
    }
    else{
        painter.setBrush(Qt::gray);
        QRect rect = QRect(WIDTH_SANJIAO, 0, this->width() - WIDTH_SANJIAO, this->height());
        painter.drawRoundedRect(rect, 5, 5);

        QPointF points[] ={
            QPointF(rect.x(), 12),
            QPointF(rect.x(), 12 + WIDTH_SANJIAO),
            QPointF(rect.x() - WIDTH_SANJIAO / 2, 12 + WIDTH_SANJIAO / 2)
        };
        painter.drawPolygon(points, 3);
    }
    return QFrame::paintEvent(event);
}

//只允许添加一次
void BubbleFrame::SetWidget(QWidget* wid){
    if(_h_layout->count() > 0){
        return;
    }else{
        _h_layout->addWidget(wid);
    }
}
