#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "global.h"

class ChatItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChatItemBase(ChatRole role, QWidget *parent = nullptr);
    void SetUserName(const QString& name);
    void SetUserIcon(const QPixmap& pix);
    void SetWidget(QWidget* w);

private:
    ChatRole m_role;
    QLabel* m_pNameLabel;
    QLabel* m_pIconLabel;
    QWidget* m_pBubble;

signals:
};

#endif // CHATITEMBASE_H
