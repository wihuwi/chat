#ifndef BUBBLEFRAME_H
#define BUBBLEFRAME_H
#include <QFrame>
#include <QHBoxLayout>
#include <QPainter>
#include "global.h"

#define WIDTH_SANJIAO 8

class BubbleFrame:public QFrame
{
    Q_OBJECT
public:
    BubbleFrame(QWidget* parent = nullptr, int margin = 3, ChatRole chatrole = ChatRole::self);
    void SetWidget(QWidget* wid);

protected:
    virtual void paintEvent(QPaintEvent* event)override;

private:
    QHBoxLayout* _h_layout;
    ChatRole _chat_role;
    int _margin;
};

#endif // BUBBLEFRAME_H
