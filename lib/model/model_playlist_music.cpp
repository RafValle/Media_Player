#include "model_playlist_music.h"

namespace model {

const QString &PlaylistMusic::name() const
{
    return _name;
}

void PlaylistMusic::setName(const QString &newName)
{
    if (_name == newName)
        return;
    _name = newName;
    emit nameChanged();
}

const QString &PlaylistMusic::getUri() const
{
    return _uri;
}

void PlaylistMusic::setUri(const QString &newUri)
{
    if (_uri == newUri)
        return;
    _uri = newUri;
    emit uriChanged();
}

const QString &PlaylistMusic::getArtists() const
{
    return _artists;
}

void PlaylistMusic::setArtists(const QString &newArtists)
{
    if (_artists == newArtists)
        return;
    _artists = newArtists;
    emit artistsChanged();
}

const QString &PlaylistMusic::album_image() const
{
    return _album_image;
}

void PlaylistMusic::setAlbum_image(const QString &newAlbum_image)
{
    if (_album_image == newAlbum_image)
        return;
    _album_image = newAlbum_image;
    emit album_imageChanged();
}

const QString &PlaylistMusic::album() const
{
    return _album;
}

void PlaylistMusic::setAlbum(const QString &newAlbum)
{
    if (_album == newAlbum)
        return;
    _album = newAlbum;
    emit albumChanged();
}

const QString &PlaylistMusic::url() const
{
    return _url;
}

void PlaylistMusic::setUrl(const QString &newUrl)
{
    if (_url == newUrl)
        return;
    _url = newUrl;
    emit urlChanged();
}

const QString &PlaylistMusic::preview_url() const
{
    return _preview_url;
}

void PlaylistMusic::setPreview_url(const QString &newPreview_url)
{
    if (_preview_url == newPreview_url)
        return;
    _preview_url = newPreview_url;
    emit preview_urlChanged();
}

const QString &PlaylistMusic::duration() const
{
    return _duration;
}

void PlaylistMusic::setDuration(const QString &newDuration)
{
    if (_duration == newDuration)
        return;
    _duration = newDuration;
    emit durationChanged();
}

long long PlaylistMusic::id() const
{
    return _id;
}

void PlaylistMusic::setId(long long newId)
{
    if (_id == newId)
        return;
    _id = newId;
    emit idChanged();
}

bool PlaylistMusic::ativo() const
{
    return _ativo;
}

void PlaylistMusic::setAtivo(bool newAtivo)
{
    if (_ativo == newAtivo)
        return;
    _ativo = newAtivo;
    emit ativoChanged();
}

const QDateTime &PlaylistMusic::dt() const
{
    return _dt;
}

void PlaylistMusic::setDt(const QDateTime &newDt)
{
    if (_dt == newDt)
        return;
    _dt = newDt;
    emit dtChanged();
}

long long PlaylistMusic::playlists_id() const
{
    return _playlists_id;
}

void PlaylistMusic::setPlaylists_id(long long newPlaylists_id)
{
    if (_playlists_id == newPlaylists_id)
        return;
    _playlists_id = newPlaylists_id;
    emit playlists_idChanged();
}

}
