#include "chatpage.h"
#include <QStyleOption>
#include "chatitembase.h"
#include "picturebubble.h"
#include "textbubble.h"
#include "ui_chatpage.h"
#include <QPainter>


ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
{
    ui->setupUi(this);
    ui->emo_lb->SetState("normal", "press", "hover", "normal", "press", "hover");
    ui->file_lb->SetState("normal", "press", "hover", "normal", "press", "hover");

    ui->receive_btn->SetState("normal", "hover", "press");
    ui->send_btn->SetState("normal", "hover", "press");
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatPage::on_send_btn_clicked()
{
    auto pTextEdit = ui->chatEdit;
    ChatRole role = ChatRole::self;
    QString userName = QStringLiteral("恋恋风辰");
    QString userIcon = ":/icons/head_1.jpg";
    const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
    for(int i=0; i<msgList.size(); ++i)
    {
        QString type = msgList[i].msgFlag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->SetUserName(userName);
        pChatItem->SetUserIcon(QPixmap(userIcon));
        QWidget *pBubble = nullptr;
        if(type == "text")
        {
            pBubble = new TextBubble(role, msgList[i].content);
        }
        else if(type == "image")
        {
            pBubble = new PictureBubble(QPixmap(msgList[i].content) , role);
        }
        else if(type == "file")
        {
        }
        if(pBubble != nullptr)
        {
            pChatItem->SetWidget(pBubble);
            ui->chat_data_list->appendChatItem(pChatItem);
        }
    }
}

