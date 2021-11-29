#ifndef DB_RESULT_H
#define DB_RESULT_H

#include <QObject>
#include <QVariant>
#include <QMap>

namespace db {

class Result : public QObject
{
    Q_OBJECT
public:
    explicit Result(QObject *parent = nullptr);

    enum Type {
        SUCESS = 1,
        ERROR  = 2,
    };

    QVariantMap mapData() const;
    void setMapData(const QVariantMap &mapData);

    void setData (const QString&name, QVariant value);
    QVariant data(const QString&name);

    Type type() const;
    void setType(const Type &type);
    void setType(const int &type);

    QByteArray toJson();

    static Result* fromJson(const QByteArray&json);
    static Result* fromJson(const QString &json);

    int requestId() const;
    void setRequestId(int requestId);

    QString message() const;
    void setMessage(const QString &message);

    bool isError();

private:
    QVariantMap _mapData;
    Type _type;
    QString _message;
    int _requestId;
};

}
#endif // DB_RESULT_H
