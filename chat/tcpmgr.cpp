#include "tcpmgr.h"
#include "usermgr.h"

TcpMgr::TcpMgr():_host(""), _port(0),_id(0), _len(0), _b_recv_pending(false) {
    connect(&_sock, &QTcpSocket::connected, this, [this](){
        emit sig_con_success(true);
    });

    connect(&_sock, &QTcpSocket::readyRead, this, [this](){
        _data.append(_sock.readAll());

        QDataStream dataStream(&_data, QIODevice::ReadOnly);

        forever{
            if(!_b_recv_pending){
                if(_data.size() < static_cast<int>(sizeof(quint16) * 2)){
                    return;
                }

                dataStream >> _id >> _len;
                _data = _data.mid(sizeof(quint16) * 2);
            }

            if(_data.size() < _len){
                _b_recv_pending = true;
                return;
            }

            _b_recv_pending = false;
            QByteArray data = _data.mid(0, _len);
            _data = _data.mid(_len);
            qDebug()<<"recv msg is: "<<data;

            HandleMsg(static_cast<ReqId>(_id), data, _len);
            qDebug()<<"handleMsg";
        }
    });

    //error
    connect(&_sock, &QTcpSocket::errorOccurred, this, [this](){
        qDebug()<<"sock err" << _sock.error();
        emit sig_con_success(static_cast<int>(ErrorCodes::ERR_NETWORK));
    });

    connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);

    InitHandlers();
}

TcpMgr::~TcpMgr(){

}

void TcpMgr::InitHandlers(){
    _handlers.insert(ReqId::ID_CHAT_LOGIN_RSP, [&](ReqId id, const QByteArray& data, quint16 len){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"ID_CHAT_LOGIN_RSP jsonDoc err";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            jsonObj["error"] = ErrorCodes::ERR_JSON;
            qDebug()<<"ID_CHAT_LOGIN_RSP jsonObj err";
            emit sig_login_failed(static_cast<int>(ERR_JSON));
            return;
        }

        if(jsonObj["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"ID_CHAT_LOGIN_RSP err: "<< jsonObj["error"];
            emit sig_login_failed(jsonObj["error"].toInt());
            return;
        }

        //emit sig_con_success(0);
        qDebug()<<"reqid:" <<id;
        UserMgr::GetInstance()->SetUid(jsonObj["uid"].toInt());
        UserMgr::GetInstance()->SetName(jsonObj["name"].toString());
        UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());

        //添加用户信息
        auto uid = jsonObj["uid"].toInt();
        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["sex"].toInt();
        auto desc = jsonObj["desc"].toString();
        auto user_info = std::make_shared<UserInfo>(uid, name, nick, icon, sex,"",desc);
        UserMgr::GetInstance()->SetUserInfo(user_info);

        //添加申请列表
        if(jsonObj.contains("apply_list")){
            UserMgr::GetInstance()->AppendApplyList(jsonObj["apply_list"].toArray());
        }

        //添加好友列表
        if(jsonObj.contains("friend_list")){
            UserMgr::GetInstance()->AppendFriendList(jsonObj["friend_list"].toArray());
        }

        emit sig_switch_chatdlg();
    });

    _handlers.insert(ReqId::ID_SEARCH_USER_RSP, [&](ReqId id, const QByteArray& data, quint16 len){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"ID_SEARCH_USER_RSP jsonDoc err";
            sig_user_search(nullptr);
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            jsonObj["error"] = ErrorCodes::ERR_JSON;
            qDebug()<<"ID_SEARCH_USER_RSP jsonObj err";
            sig_user_search(nullptr);
            return;
        }

        if(jsonObj["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"ID_SEARCH_USER_RSP err: "<< jsonObj["error"];
            sig_user_search(nullptr);
            return;
        }

        auto search_info =  std::make_shared<SearchInfo>(jsonObj["uid"].toInt(), jsonObj["name"].toString(),
                                                        jsonObj["nick"].toString(), jsonObj["desc"].toString(),
                                                        jsonObj["sex"].toInt(), jsonObj["icon"].toString());

        emit sig_user_search(search_info);
    });

    _handlers.insert(ReqId::ID_ADD_FRIEND_RSP, [&](ReqId id, const QByteArray& data, quint16 len){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"ID_ADD_FRIEND_RSP jsonDoc err";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            jsonObj["error"] = ErrorCodes::ERR_JSON;
            qDebug()<<"ID_ADD_FRIEND_RSP jsonObj err";
            return;
        }

        if(jsonObj["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"ID_ADD_FRIEND_RSP err: "<< jsonObj["error"];
            return;
        }

        qDebug()<<"ID_ADD_FRIEND_RSP Success";
    });

    _handlers.insert(ReqId::ID_NOTIFY_ADD_FRIEND_REQ, [&](ReqId id, const QByteArray& data, quint16 len){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"ID_NOTIFY_ADD_FRIEND_REQ jsonDoc err";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            jsonObj["error"] = ErrorCodes::ERR_JSON;
            qDebug()<<"ID_NOTIFY_ADD_FRIEND_REQ jsonObj err";
            return;
        }

        if(jsonObj["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"ID_NOTIFY_ADD_FRIEND_REQ err: "<< jsonObj["error"];
            return;
        }

        int from_uid = jsonObj["applyuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString desc = jsonObj["desc"].toString();
        QString icon = jsonObj["icon"].toString();
        QString nick = jsonObj["nick"].toString();
        int sex = jsonObj["sex"].toInt();
        std::shared_ptr<AddFriendApply> applyInfo = std::make_shared<AddFriendApply>(from_uid, name, desc, icon, nick, sex);

        emit sig_friend_apply(applyInfo);
        qDebug()<<"ID_NOTIFY_ADD_FRIEND_REQ Success";
    });

    _handlers.insert(ReqId::ID_NOTIFY_AUTH_FRIEND_REQ, [&](ReqId id, const QByteArray& data, quint16 len){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"ID_NOTIFY_AUTH_FRIEND_REQ jsonDoc err";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            jsonObj["error"] = ErrorCodes::ERR_JSON;
            qDebug()<<"ID_NOTIFY_AUTH_FRIEND_REQ jsonObj err";
            return;
        }

        if(jsonObj["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"ID_NOTIFY_AUTH_FRIEND_REQ err: "<< jsonObj["error"];
            return;
        }

        int from_uid = jsonObj["fromuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString nick = jsonObj["nick"].toString();
        QString icon = jsonObj["icon"].toString();
        int sex = jsonObj["sex"].toInt();

        auto auth_info = std::make_shared<AuthInfo>(from_uid,name,
                                                    nick, icon, sex);

        emit sig_add_auth_friend(auth_info);
        qDebug()<<"ID_NOTIFY_AUTH_FRIEND_REQ Success";
    });

    _handlers.insert(ReqId::ID_AUTH_FRIEND_RSP, [&](ReqId id, const QByteArray& data, quint16 len){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"ID_AUTH_FRIEND_RSP jsonDoc err";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            jsonObj["error"] = ErrorCodes::ERR_JSON;
            qDebug()<<"ID_AUTH_FRIEND_RSP jsonObj err";
            return;
        }

        if(jsonObj["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"ID_AUTH_FRIEND_RSP err: "<< jsonObj["error"];
            return;
        }

        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["sex"].toInt();
        auto uid = jsonObj["uid"].toInt();
        auto rsp = std::make_shared<AuthRsp>(uid, name, nick, icon, sex);
        emit sig_auth_rsp(rsp);

        qDebug()<<"ID_AUTH_FRIEND_RSP Success";
    });

    _handlers.insert(ReqId::ID_TEXT_CHAT_MSG_RSP, [&](ReqId id, const QByteArray& data, quint16 len){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"ID_TEXT_CHAT_MSG_RSP jsonDoc err";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            jsonObj["error"] = ErrorCodes::ERR_JSON;
            qDebug()<<"ID_TEXT_CHAT_MSG_RSP jsonObj err";
            return;
        }

        if(jsonObj["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"ID_TEXT_CHAT_MSG_RSP err: "<< jsonObj["error"];
            return;
        }

        qDebug()<<"ID_TEXT_CHAT_MSG_RSP Success";
    });

    _handlers.insert(ReqId::ID_NOTIFY_TEXT_CHAT_MSG_REQ, [&](ReqId id, const QByteArray& data, quint16 len){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"ID_NOTIFY_TEXT_CHAT_MSG_REQ jsonDoc err";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            jsonObj["error"] = ErrorCodes::ERR_JSON;
            qDebug()<<"ID_NOTIFY_TEXT_CHAT_MSG_REQ jsonObj err";
            return;
        }

        if(jsonObj["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"ID_NOTIFY_TEXT_CHAT_MSG_REQ err: "<< jsonObj["error"];
            return;
        }

        auto msg_ptr = std::make_shared<TextChatMsg>(jsonObj["fromuid"].toInt(),
                                                     jsonObj["touid"].toInt(),jsonObj["text_array"].toArray());
        emit sig_text_chat_msg(msg_ptr);

        qDebug()<<"ID_NOTIFY_TEXT_CHAT_MSG_REQ Success";
    });
}

void TcpMgr::HandleMsg(ReqId id, const QByteArray& data, quint16 len){
    auto it = _handlers.find(id);
    if(it == _handlers.end()){
        qDebug()<<"no handler";
        return;
    }
    it.value()(id, data, len);
}

void TcpMgr::slot_connect_tcp(ServerInfo info){
    _port = static_cast<quint16>(info.Port.toInt());
    _host = info.Host;
    qDebug()<<_host<<_port;
    _sock.connectToHost(_host, _port);
}

void TcpMgr::slot_send_data(ReqId reqId, QByteArray dataBytes){
    uint16_t id = reqId;

    // 计算长度（使用网络字节序转换）
    quint16 len = static_cast<quint16>(dataBytes.length());

    // 创建一个QByteArray用于存储要发送的所有数据
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    // 设置数据流使用网络字节序
    out.setByteOrder(QDataStream::BigEndian);

    // 写入ID和长度
    out << id << len;

    // 添加字符串数据
    block.append(dataBytes);

    // 发送数据
    _sock.write(block);
    qDebug() << "tcp mgr send byte data is " << block ;
}
