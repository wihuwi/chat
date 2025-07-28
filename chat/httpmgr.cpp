#include "httpmgr.h"


HttpMgr::HttpMgr() {
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);
}


void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod){

    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply = _manager.post(request, data);
    auto self = shared_from_this();
    QObject::connect(reply, &QNetworkReply::finished, this, [self, reply, req_id, mod](){
        if(reply->error() != QNetworkReply::NoError){
            qDebug()<< reply->errorString();
            //通知完成
            emit self->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            reply->deleteLater();
            return;
        }

        QString res = reply->readAll();
        emit self->sig_http_finish(req_id, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
    });
}

void HttpMgr::slot_http_finish(ReqId req_id, QString data, ErrorCodes err, Modules mod){
    switch(mod){
    case Modules::REGISTERMOD:{
        emit this->sig_reg_mod_finish(req_id, data, err);
    }
    case Modules::RESETMOD:{
        emit this->sig_reset_mod_finish(req_id, data, err);
    }
    }
}
