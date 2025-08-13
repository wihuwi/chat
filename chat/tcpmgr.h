#ifndef TCPMGR_H
#define TCPMGR_H

#include "singleton.h"
#include "global.h"
#include "userdata.h"

#include <QTcpSocket>
#include <functional>
#include <QJsonDocument>
#include <QJsonObject>

class TcpMgr:public QObject, public std::enable_shared_from_this<TcpMgr>, public Singleton<TcpMgr>
{
    Q_OBJECT
    //QOBJECT 不需要加;
    friend Singleton<TcpMgr>;
public:
    ~TcpMgr();


private:
    TcpMgr();
    void InitHandlers();
    void HandleMsg(ReqId id, const QByteArray& data, quint16 len);
    QTcpSocket _sock;
    QString _host;
    quint16 _port;
    QByteArray _data;
    quint16 _id;
    quint16 _len;
    bool _b_recv_pending;
    QMap<ReqId, std::function<void(ReqId id, const QByteArray& data, quint16 len)>> _handlers;

public slots:
    void slot_connect_tcp(ServerInfo info);
    void slot_send_data(ReqId id, QByteArray data);
    //因为发送数据的信号不一定由tcpMgr调用，而且tcpMgr是用来管理进行连接，目前没有接收发送数据的需求
signals:
    void sig_login_failed(int err);
    void sig_con_success(bool success);
    void sig_send_data(ReqId id, QByteArray data);
    void sig_switch_chatdlg();
    void sig_user_search(std::shared_ptr<SearchInfo>);
    void sig_friend_apply(std::shared_ptr<AddFriendApply>);
    void sig_add_auth_friend(std::shared_ptr<AuthInfo>);
    void sig_auth_rsp(std::shared_ptr<AuthRsp>);
};

#endif // TCPMGR_H
