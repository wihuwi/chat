#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QString>
#include <QUrl>
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

#include "singleton.h"
#include "global.h"



class HttpMgr:public QObject, public Singleton<HttpMgr>,
                public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT

public:
    ~HttpMgr() = default;
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);
private:
    friend class Singleton<HttpMgr>;
    HttpMgr();
    QNetworkAccessManager _manager;
public slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    //注册模块http相关请求完成发送此信号
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void sig_reset_mod_finish(ReqId id, QString res, ErrorCodes err);
    void sig_login_mod_finish(ReqId id, QString res, ErrorCodes err);
};
#endif // HTTPMGR_H
