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
    void SetState(QString nor, QString nor_press, QString nor_hover, QString sel, QString sel_press, QString sel_hover);
    void ResetNormalState();
    bool SetCurState(ClickLbState state);

protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    ClickLbState _curState;
    QString _normal;
    QString _normal_hover;
    QString _normal_press;
    QString _select;
    QString _select_hover;
    QString _select_press;

signals:
    void clicked(QString lbtext, ClickLbState state);
};

#endif // CLICKEDLABEL_H
