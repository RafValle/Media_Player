#pragma once

#include <QThread>
#include <QTimer>

#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "network_structs.h"

namespace network {

class RequestJob : public QThread
{
    Q_OBJECT
public:
    explicit RequestJob(Request&request):QThread(nullptr){
        this->setObjectName(QStringLiteral("RequestJob::%1").arg(request.url.toString()));
        this->_request=&request;        
        QObject::connect(this, &RequestJob::started, this, &RequestJob::onStarted);
        QObject::connect(this, &RequestJob::ended, this, &RequestJob::onEnded);
    }

    ~RequestJob(){
        if(this->_loop!=nullptr)
            this->_loop->deleteLater();
        if(this->_nam!=nullptr)
            this->_nam->deleteLater();
        if(this->_rep!=nullptr)
            this->_rep->deleteLater();
        if(this->_out!=nullptr)
            this->_out->deleteLater();
    }
    void run() override;

    void timeoutStart();
    void timeoutStop();

signals:
    void end();
    void started();
    void ended();

public slots:
    void onEnded();
    void onStarted();
    void requestFunc();
    void onReplyTimeout();
    void onReplyDelete();
    void onReplyFinish();
    void onReplyError(QNetworkReply::NetworkError e);
    void onFinish();


private:
    Request*_request=nullptr;

    QEventLoop *_loop=nullptr;
    QNetworkReply *_rep = nullptr;
    QNetworkAccessManager*_nam = nullptr;
    QTimer*_out=nullptr;
};

}

