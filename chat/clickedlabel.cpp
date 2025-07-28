#include "clickedlabel.h"

ClickedLabel::ClickedLabel(QWidget* parent):QLabel(parent), _curState(ClickLbState::Normal){
    this->setCursor(Qt::CursorShape::PointingHandCursor);
}

ClickedLabel::~ClickedLabel(){

}

void ClickedLabel::enterEvent(QEnterEvent* event){
    if(_curState == ClickLbState::Normal){
        this->setProperty("state", _normal_hover);
        repolish(this);
        update();
    }
    else{
        this->setProperty("state", _select_hover);
        repolish(this);
        update();
    }
    QLabel::enterEvent(event);
}

void ClickedLabel::leaveEvent(QEvent* event){
    if(_curState == ClickLbState::Normal){
        this->setProperty("state", _normal);
        repolish(this);
        update();
    }
    else{
        this->setProperty("state", _select);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void ClickedLabel::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        if(_curState == Normal){
            this->setProperty("state", _normal);
            repolish(this);
            _curState = ClickLbState::Selected;
            update();
        }
        else{
            this->setProperty("state", _select);
            repolish(this);
            _curState = ClickLbState::Normal;
            update();
        }

        emit clicked();
    }
    QLabel::mousePressEvent(event);
}

void ClickedLabel::SetState(QString nor, QString nor_hover, QString sel, QString sel_hover){
    _normal = nor;
    _normal_hover = nor_hover;
    _select = sel;
    _select_hover = sel_hover;

    this->setProperty("state", nor);
    repolish(this);
    update();
}

ClickLbState ClickedLabel::GetState(){
    return _curState;
}
