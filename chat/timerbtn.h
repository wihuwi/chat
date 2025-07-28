#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QPushButton>
#include <QTimer>
#include <QMouseEvent>

class TimerBtn:public QPushButton
{
    Q_OBJECT;
public:
    TimerBtn(QWidget* parent);
    ~TimerBtn();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    QTimer* _timer;
    int _count;
};

#endif // TIMERBTN_H
