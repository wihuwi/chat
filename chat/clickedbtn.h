#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H
#include <QPushButton>
#include <QMouseEvent>
#include <QEnterEvent>

#include "global.h"

class ClickedBtn: public QPushButton
{
    Q_OBJECT
public:
    ClickedBtn(QWidget* parent);
    ~ClickedBtn();
    void SetState(const QString normal, const QString hover, const QString _press);

protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
private:
    QString _normal;
    QString _hover;
    QString _press;
};

#endif // CLICKEDBTN_H
