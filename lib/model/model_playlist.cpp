#include "model_playlist.h"

namespace model {

long long Playlist::id() const
{
    return _id;
}

void Playlist::setId(long long newId)
{
    if (_id == newId)
        return;
    _id = newId;
    emit idChanged();
}

bool Playlist::ativo() const
{
    return _ativo;
}

void Playlist::setAtivo(bool newAtivo)
{
    if (_ativo == newAtivo)
        return;
    _ativo = newAtivo;
    emit ativoChanged();
}

const QString &Playlist::name() const
{
    return _name;
}

void Playlist::setName(const QString &newName)
{
    if (_name == newName)
        return;
    _name = newName;
    emit nameChanged();
}

const QDateTime &Playlist::dt() const
{
    return _dt;
}

void Playlist::setDt(const QDateTime &newDt)
{
    if (_dt == newDt)
        return;
    _dt = newDt;
    emit dtChanged();
}

}
