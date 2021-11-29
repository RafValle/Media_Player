#ifndef NETWORK_STRUCTS_H
#define NETWORK_STRUCTS_H

#include <QObject>
#include <QUrl>
#include <QVariantMap>
#include <QDateTime>

namespace network {


typedef struct Response {
    QMap<QByteArray,QByteArray> headers;
    QByteArray body;
    int code;
    QString codeString;
    int qtCode;
    QString qtCodeString;
} Response;

typedef struct Request {
    enum Method {
        POST ,
        GET
    };
    QUrl url;
    QMap<QByteArray,QByteArray> headers;
    QVariantMap params;
    QByteArray body;
    Method method = GET;
    int timeout = 30000;// 30 * 1000 msec;
    QString start;
    QString end;
    Response response;
} Request;

}
#endif // NETWORK_STRUCTS_H
