#include "searchlist.h"
#include<QScrollBar>
#include "adduseritem.h"
// #include "invaliditem.h"
#include "findsuccessdialog.h"
#include "chatuserwid.h"
#include "tcpmgr.h"
#include "customizeedit.h"
//#include "findfaildlg.h"
//#include "loadingdlg.h"
#include "userdata.h"
#include "usermgr.h"
#include "findfaildlg.h"

SearchList::SearchList(QWidget *parent):QListWidget(parent),_find_dlg(nullptr), _search_edit(nullptr), _send_pending(false)
{
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    this->viewport()->installEventFilter(this);
    //连接点击的信号和槽
    connect(this, &QListWidget::itemClicked, this, &SearchList::slot_item_clicked);
    //添加条目
    addTipItem();
    //连接搜索条目
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_user_search, this, &SearchList::slot_user_search);
}

void SearchList::CloseFindDlg()
{
    if(_find_dlg){
        _find_dlg->hide();
        _find_dlg = nullptr;
    }
}

void SearchList::SetSearchEdit(QWidget *edit)
{
    _search_edit = edit;
}


bool SearchList::eventFilter(QObject* o, QEvent* e){
    if(o == this->viewport()){
        if(e->type() == QEvent::Enter){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }
        else if(e->type() == QEvent::Leave){
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
        //return true;
        //这里不能直接返回！！！！！
    }
    if(o == this->viewport() && e->type() == QEvent::Wheel){
        QWheelEvent* event = static_cast<QWheelEvent*>(e);
        auto step = event->angleDelta().y() / 8 / 15;

        QScrollBar* v_bar = this->verticalScrollBar();
        v_bar->setValue(v_bar->value() - step);
        return true;
    }
    return QListWidget::eventFilter(o, e);
}

void SearchList::waitPending(bool pending)
{

}

void SearchList::addTipItem()
{

    auto *invalid_item = new QWidget();
    QListWidgetItem *item_tmp = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item_tmp->setSizeHint(QSize(250,10));
    this->addItem(item_tmp);
    invalid_item->setObjectName("invalid_item");
    this->setItemWidget(item_tmp, invalid_item);
    //item_tmp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item_tmp->setFlags(item_tmp->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);



    auto *add_user_item = new AddUserItem();

    QListWidgetItem *item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(add_user_item->sizeHint());
    this->addItem(item);
    this->setItemWidget(item, add_user_item);
}

void SearchList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = this->itemWidget(item); // 获取自定义widget对象
    if(!widget){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == ListItemType::INVALID_ITEM){
        qDebug()<< "slot invalid item clicked ";
        return;
    }

    if(customItem->GetItemType() == ListItemType::ADD_USER_TIP_ITEM){
        qDebug()<<"ADD_USER_TIP_ITEM";
        //todo...
        //pending ...
        if(!_search_edit){
            return;
        }
        auto search_edit = dynamic_cast<CustomizeEdit*>(_search_edit);
        QString uid_str = search_edit->text();
        QJsonObject jsonObj;
        jsonObj["uid"] = uid_str;
        QJsonDocument jsonDoc(jsonObj);
        TcpMgr::GetInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ, jsonDoc.toJson(QJsonDocument::Compact));

        // _find_dlg = std::make_shared<FindSuccessDialog>(this);
        // std::shared_ptr<SearchInfo> si = std::make_shared<SearchInfo>(0, "1111", "222", "3333", 1, "444");
        // std::dynamic_pointer_cast<FindSuccessDialog>(_find_dlg)->SetSearchInfo(si);
        // _find_dlg->show();
        return;
    }

    CloseFindDlg();
}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
    if(si == nullptr){
        _find_dlg = std::make_shared<FindFailDlg>(this);
    }else{
        _find_dlg = std::make_shared<FindSuccessDialog>(this);
        std::dynamic_pointer_cast<FindSuccessDialog>(_find_dlg)->SetSearchInfo(si);
    }
    _find_dlg->show();
}
