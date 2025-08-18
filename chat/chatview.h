#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QVBoxLayout>
#include <QScrollArea>
#include <QWheelEvent>
#include <QWidget>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>

class ChatView : public QWidget
{
    Q_OBJECT
public:
    explicit ChatView(QWidget *parent = nullptr);
    void appendChatItem(QWidget* item);
    void removeAllItem();

protected:
    virtual bool eventFilter(QObject *o, QEvent *e);
    /*virtual void enterEvent(QEnterEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void wheelEvent(QWheelEvent* event);*/
    virtual void paintEevent(QPaintEvent* event);

private:
    void initStyleSheet();

    QVBoxLayout* m_pVl;
    QScrollArea* m_pScrollArea;
    bool isAppended;

private slots:
    void onVScrollBarMoved(int min, int max);

signals:
};

#endif // CHATVIEW_H
