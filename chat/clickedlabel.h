#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>
#include <QEnterEvent>
#include <QMouseEvent>
#include <QEvent>

#include "global.h"

class ClickedLabel: public QLabel
{
    Q_OBJECT;
public:
    ClickedLabel(QWidget* parent);
    ~ClickedLabel();
    ClickLbState GetState();
    void SetState(QString nor, QString nor_hover, QString sel, QString sel_hover);

protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    ClickLbState _curState;
    QString _normal;
    QString _normal_hover;
    QString _select;
    QString _select_hover;

signals:
    void clicked();
};

#endif // CLICKEDLABEL_H
