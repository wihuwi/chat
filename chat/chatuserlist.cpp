#include "chatuserlist.h"

ChatUserList::ChatUserList(QWidget* parent): QListWidget(parent) {
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
}

ChatUserList::~ChatUserList(){

}

void ChatUserList::enterEvent(QEnterEvent* event){
    this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
}

void ChatUserList::leaveEvent(QEvent* event){
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ChatUserList::wheelEvent(QWheelEvent* event){
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15; // 计算滚动步数

    // 设置滚动幅度
    this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

    // 检查是否滚动到底部
    QScrollBar *scrollBar = this->verticalScrollBar();
    int maxScrollValue = scrollBar->maximum();
    int currentValue = scrollBar->value();
    //int pageSize = 10; // 每页加载的联系人数量
    if(maxScrollValue - currentValue < 0){
        qDebug()<<"load more user";
        emit sig_loading_chat_user();
    }
}
