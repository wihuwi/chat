#include "tcpmgr.h"

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
        }
    });

    //error
    connect(&_sock, &QTcpSocket::errorOccurred, this, [this](){
        qDebug()<<"sock err" << _sock.error();
        emit sig_con_success(ErrorCodes::ERR_NETWORK);
    });

    connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);

    InitHandlers();
}

void TcpMgr::InitHandlers(){
    _handlers.insert(ReqId::ID_CHAT_LOGIN_RSP, [this](ReqId id, const QByteArray& data, quint16 len){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if(jsonDoc.isNull()){
            qDebug()<<"tcpMgr jsonDoc err";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error")){
            jsonObj["error"] = ErrorCodes::ERR_JSON;
            qDebug()<<"tcpMgr jsonDoc err";
            emit sig_login_failed(ERR_JSON);
            return;
        }

        if(jsonObj["error"].toInt() != ErrorCodes::SUCCESS){
            qDebug()<<"tcpMgr err: "<< jsonObj["error"];
            emit sig_login_failed(jsonObj["error"].toInt());
            return;
        }

        //emit sig_con_success(0);
        emit sig_switch_chatdlg();
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
    _sock.connectToHost(_host, _port);
}

void TcpMgr::slot_send_data(ReqId req_id, const QByteArray& data){
    quint16 id = static_cast<quint16>(req_id);
    quint16 len = static_cast<quint16>(data.size());
    QByteArray sendData;
    QDataStream out(&sendData, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out << id << len;
    sendData.append(data);
    _sock.write(sendData);
}
