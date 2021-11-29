#ifndef MODEL_REQUESTS_H
#define MODEL_REQUESTS_H

#include <QObject>
#include <QDateTime>

#include "db_macros.h"

namespace model {

class Requests : public QObject
{
    Q_OBJECT
public:

    PK_FIELD(id)
    TABLE_NAME(requests)
    TABLE_FIELD(id)
    TABLE_FIELD(dt)
    TABLE_FIELD(ativo)
    TABLE_FIELD(status)
    TABLE_FIELD(body)
    TABLE_FIELD(url)

    Q_PROPERTY(long long  id     READ id     WRITE setId     NOTIFY idChanged     )
    Q_PROPERTY(bool       ativo  READ ativo  WRITE setAtivo  NOTIFY ativoChanged  )
    Q_PROPERTY(QDateTime  dt     READ dt     WRITE setDt     NOTIFY dtChanged     )
    Q_PROPERTY(int        status READ status WRITE setStatus NOTIFY statusChanged )
    Q_PROPERTY(QString    body   READ body   WRITE setBody   NOTIFY bodyChanged   )
    Q_PROPERTY(QString    url    READ url    WRITE setUrl    NOTIFY urlChanged    )

    Q_INVOKABLE explicit Requests(QObject *parent = nullptr):QObject(parent){}
    ~Requests(){}

    long long id() const;
    void setId(long long newId);

    bool ativo() const;
    void setAtivo(bool newAtivo);

    const QDateTime &dt() const;
    void setDt(const QDateTime &newDt);

    int status() const;
    void setStatus(int newStatus);

    const QString &body() const;
    void setBody(const QString &newBody);

    const QString &url() const;
    void setUrl(const QString &newUrl);

signals:
    void idChanged();
    void ativoChanged();
    void dtChanged();
    void statusChanged();
    void bodyChanged();
    void urlChanged();

private:
    long long _id=0;
    bool _ativo=true;
    QDateTime _dt=QDateTime::currentDateTime();
    int _status=0;
    QString _body="";
    QString _url="";
};
}
#endif // MODEL_REQUESTS_H
