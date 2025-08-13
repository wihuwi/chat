#ifndef PICTUREBUBBLE_H
#define PICTUREBUBBLE_H
#include "bubbleframe.h"
#include "global.h"

class PictureBubble:public BubbleFrame
{
    Q_OBJECT
public:
    PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent = nullptr);
};

#endif // PICTUREBUBBLE_H
