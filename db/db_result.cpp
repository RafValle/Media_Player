#include "db_result.h"

#include <QJsonDocument>

namespace db {

Result::Result(QObject *parent) : QObject(parent)
{}

QVariantMap Result::mapData() const
{
    return _mapData;
}

void Result::setMapData(const QVariantMap &mapData)
{
    _mapData = mapData;
}

void Result::setData(const QString&name, QVariant value)
{
    _mapData.insert(name, value);
}

QVariant Result::data(const QString&name)
{
    return _mapData.value(name);
}

Result::Type Result::type() const
{
    return _type;
}

void Result::setType(const int &type)
{
    _type = Result::Type(type);
}

int Result::requestId() const
{
    return _requestId;
}

void Result::setRequestId(int requestId)
{
    _requestId = requestId;
}

QString Result::message() const
{
    return _message;
}

void Result::setMessage(const QString &message)
{
    _message = message;
}

bool Result::isError()
{
    return (_type != Result::SUCESS);
}

QByteArray Result::toJson()
{
    QVariantMap dataJson =
    {
        {QStringLiteral("type")   , this->type()},
        {QStringLiteral("message"), this->message()},
        {QStringLiteral("data")   , this->mapData()}
    };
    return QJsonDocument::fromVariant(dataJson).toJson(QJsonDocument::Compact);
}

Result *Result::fromJson(const QByteArray&json)
{
    QJsonParseError localParser;
    QJsonDocument doc = QJsonDocument::fromJson(json, &localParser);

    if (localParser.error != QJsonParseError::NoError){
#ifdef QT_DEBUG
//        qDebug() << __PRETTY_FUNCTION__ <<  localParser.errorString();
#endif
        return nullptr;
    }

    QVariantMap dataDoc = doc.toVariant().toMap();

    if (dataDoc.isEmpty()){
        return nullptr;
    }

    Result* resultado = new Result(nullptr); // TODO INCLUIR UM GERENCIADOR OU UM FACTORY PARA CONTROLAR A MEMÓRIA E SETAR PAI A ESSES OBJETOS;
    resultado->setType(dataDoc.value(QStringLiteral("type")).toInt());
    resultado->setMessage(dataDoc.value(QStringLiteral("message")).toString());
    resultado->setMapData(dataDoc.value(QStringLiteral("data")).toMap());
    return resultado;
}
}
