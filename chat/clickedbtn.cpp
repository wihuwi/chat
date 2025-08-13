#include "clickedbtn.h"

ClickedBtn::ClickedBtn(QWidget* parent):QPushButton(parent) {
    setCursor(Qt::PointingHandCursor);
}

ClickedBtn::~ClickedBtn(){

}

void ClickedBtn::SetState(const QString normal, const QString hover, const QString press){
    _normal = normal;
    _hover = hover;
    _press = press;
    setProperty("state", normal);
    repolish(this);
    update();
}

void ClickedBtn::enterEvent(QEnterEvent* event){
    setProperty("state", _hover);
    repolish(this);
    update();
    QPushButton::enterEvent(event);
}

void ClickedBtn::leaveEvent(QEvent* event){
    setProperty("state", _normal);
    repolish(this);
    update();
    QPushButton::leaveEvent(event);
}

void ClickedBtn::mousePressEvent(QMouseEvent* event){
    setProperty("state", _press);
    repolish(this);
    update();
    QPushButton::mousePressEvent(event);
}

void ClickedBtn::mouseReleaseEvent(QMouseEvent* event){
    setProperty("state", _hover);
    repolish(this);
    update();
    QPushButton::mouseReleaseEvent(event);
}
