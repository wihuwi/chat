#include "clickedoncelabel.h"

ClickedOnceLabel::ClickedOnceLabel(QWidget *parent):QLabel(parent)
{

}

void ClickedOnceLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton){
        emit clicked(this->text());
        return;
    }
    QLabel::mouseReleaseEvent(ev);
}
