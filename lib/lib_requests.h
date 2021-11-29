#ifndef LIB_REQUESTS_H
#define LIB_REQUESTS_H

#include <QObject>
#include <QVariant>
#include <QDateTime>
#include <QJsonDocument>

namespace lib {
class Requests: public QObject
{
    Q_OBJECT
public:
    static Requests& instance();

    QVariant search(const QString q, int offset);
    QVariant markets();
    QVariant genreSeeds();
    QVariant categories();

private:
    explicit Requests(QObject *parent = nullptr): QObject(parent){}
    ~Requests(){}

    QVariant baseRequest(QByteArray route);

    bool getToken();
    bool isTokenValid();
    bool checkTokenValid();

    QByteArray  _token;
    QDateTime   _validation;
    QVariantMap _accessToken;
};
}

#endif // LIB_REQUESTS_H
