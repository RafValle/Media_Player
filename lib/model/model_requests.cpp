#include "model_requests.h"

namespace model {

long long Requests::id() const
{
    return _id;
}

void Requests::setId(long long newId)
{
    if (_id == newId)
        return;
    _id = newId;
    emit idChanged();
}

bool Requests::ativo() const
{
    return _ativo;
}

void Requests::setAtivo(bool newAtivo)
{
    if (_ativo == newAtivo)
        return;
    _ativo = newAtivo;
    emit ativoChanged();
}

const QDateTime &Requests::dt() const
{
    return _dt;
}

void Requests::setDt(const QDateTime &newDt)
{
    if (_dt == newDt)
        return;
    _dt = newDt;
    emit dtChanged();
}

int Requests::status() const
{
    return _status;
}

void Requests::setStatus(int newStatus)
{
    if (_status == newStatus)
        return;
    _status = newStatus;
    emit statusChanged();
}

const QString &Requests::body() const
{
    return _body;
}

void Requests::setBody(const QString &newBody)
{
    if (_body == newBody)
        return;
    _body = newBody;
    emit bodyChanged();
}

const QString &Requests::url() const
{
    return _url;
}

void Requests::setUrl(const QString &newUrl)
{
    if (_url == newUrl)
        return;
    _url = newUrl;
    emit urlChanged();
}

}
