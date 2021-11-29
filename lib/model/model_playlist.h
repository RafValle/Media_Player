#ifndef MODEL_PLAYLIST_H
#define MODEL_PLAYLIST_H

#include <QObject>
#include <QDateTime>

#include "db_macros.h"

namespace model {

class Playlist: public QObject
{
    Q_OBJECT
public:
    PK_FIELD(id)
    TABLE_NAME(playlist)
    TABLE_FIELD(id)
    TABLE_FIELD(dt)
    TABLE_FIELD(ativo)
    TABLE_FIELD(name)

    Q_PROPERTY(long long id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QDateTime dt READ dt WRITE setDt NOTIFY dtChanged)
    Q_PROPERTY(bool ativo READ ativo WRITE setAtivo NOTIFY ativoChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    Q_INVOKABLE explicit Playlist(QObject *parent = nullptr):QObject(parent){}
    ~Playlist(){}

    long long id() const;
    void setId(long long newId);

    bool ativo() const;
    void setAtivo(bool newAtivo);

    const QDateTime &dt() const;
    void setDt(const QDateTime &newDt);

    const QString &name() const;
    void setName(const QString &newName);

signals:

    void idChanged();
    void dtChanged();
    void ativoChanged();
    void nameChanged();

private:
    long long _id=0;
    bool _ativo=true;
    QDateTime _dt=QDateTime::currentDateTime();
    QString _name="";
};

}

#endif // MODEL_PLAYLIST_H
