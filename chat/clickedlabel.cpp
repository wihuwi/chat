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
            this->setProperty("state", _select_press);
            repolish(this);
            _curState = ClickLbState::Selected;
            update();
        }
        else{
            this->setProperty("state", _normal_press);
            repolish(this);
            _curState = ClickLbState::Normal;
            update();
        }
        return;
    }
    QLabel::mousePressEvent(event);
}

void ClickedLabel::mouseReleaseEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        if(_curState == Normal){
            this->setProperty("state", _normal_hover);
            repolish(this);
            update();
        }
        else{
            this->setProperty("state", _select_hover);
            repolish(this);
            update();
        }
        emit clicked(this->text(), _curState);
        return;
    }
}

void ClickedLabel::SetState(QString nor, QString nor_press, QString nor_hover, QString sel, QString sel_press, QString sel_hover){
    _normal = nor;
    _normal_press = nor_press;
    _normal_hover = nor_hover;
    _select = sel;
    _select_press = sel_press;
    _select_hover = sel_hover;

    this->setProperty("state", nor);
    repolish(this);
    update();
}

bool ClickedLabel::SetCurState(ClickLbState state)
{
    _curState = state;
    if (_curState == ClickLbState::Normal) {
        setProperty("state", _normal);
        repolish(this);
    }
    else if (_curState == ClickLbState::Selected) {
        setProperty("state", _select);
        repolish(this);
    }

    return true;
}

void ClickedLabel::ResetNormalState()
{
    _curState = ClickLbState::Normal;
    setProperty("state", _normal);
    repolish(this);
}

ClickLbState ClickedLabel::GetState(){
    return _curState;
}
