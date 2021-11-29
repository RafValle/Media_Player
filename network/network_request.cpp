#include "network_request.h"

namespace  network {

void RequestJob::run()
{
    this->exec();
}

void RequestJob::requestFunc()
{
    if(this->_nam==nullptr)
        this->_nam = new QNetworkAccessManager(this);

    QNetworkRequest _req(this->_request->url);
    QMapIterator<QByteArray,QByteArray> i(this->_request->headers);
    while (i.hasNext()) {
         i.next();
        const auto&k=i.key();
        const auto&v=i.value();
        _req.setRawHeader(k, v);
    }

    if (this->_request->method == this->_request->GET)
        this->_rep =this->_nam->get(_req);
    else if (this->_request->method == this->_request->POST)
        this->_rep =this->_nam ->post(_req, this->_request->body);
    else {
        qDebug() << "not implemented";
    }

    if(this->_rep){
        this->timeoutStart();

        this->_request->start = QDateTime::currentDateTime().toString();
        if(this->_loop==nullptr)
            this->_loop = new QEventLoop(this);

        QObject::connect(this->_rep, &QNetworkReply::destroyed, this, &RequestJob::onReplyDelete);
        QObject::connect(this->_rep, &QNetworkReply::finished, this, &RequestJob::onReplyFinish);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        QObject::connect(this->_rep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred), this, &RequestJob::onReplyError);
#else
        QObject::connect(this->_rep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &RequestJob::onReplyError);
#endif
        QObject::connect(this, &RequestJob::end, this->_loop, &QEventLoop::quit);

        this->_loop->exec();
    }
}

void RequestJob::timeoutStart(){
    if (!this->_out){
        this->_out = new QTimer(this);
        this->_out->setSingleShot(true);
        QObject::connect(this->_out, &QTimer::timeout, this, &RequestJob::onReplyTimeout);
    }
    this->_out->start(this->_request->timeout);                
}

void RequestJob::timeoutStop(){
    if(this->_out!=nullptr){
        QObject::disconnect(this->_out, &QTimer::timeout, this, &RequestJob::onReplyTimeout);
        this->_out->deleteLater();
    }
}

void RequestJob::onEnded()
{
    this->quit();
}

void RequestJob::onStarted()
{
    this->requestFunc();
}

void RequestJob::onReplyError(QNetworkReply::NetworkError e){
    qDebug() << __PRETTY_FUNCTION__ << e;
}

void RequestJob::onReplyFinish(){
    if(this->_rep!=nullptr){
        if (_out->isActive())
            _out->stop();
        this->onFinish();
        emit end();
    }
}

void RequestJob::onReplyDelete(){
    this->_rep=nullptr;
}

void RequestJob::onReplyTimeout(){
    if(this->_rep!=nullptr){
        if (this->_rep->isRunning()){
            this->_rep->abort();
            this->_request->response.qtCode = QNetworkReply::TimeoutError;
        }
    }
}

void RequestJob::onFinish(){
    this->_request->end = QDateTime::currentDateTime().toString();
    this->_request->response.qtCode = this->_rep->error();
    this->_request->response.qtCodeString = this->_rep->errorString();
    this->_request->response.body = this->_rep->readAll();
    this->_request->response.code = this->_rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    this->_request->response.codeString = this->_rep->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    for(auto&v:this->_rep->rawHeaderPairs())
        this->_request->response.headers.insert(v.first, v.second);
    emit ended();
}

}
