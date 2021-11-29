#ifndef MODEL_PLAYLIST_MUSIC_H
#define MODEL_PLAYLIST_MUSIC_H

#include <QObject>
#include <QDateTime>
namespace model {

class PlaylistMusic: public QObject
{
    Q_OBJECT
public:

    Q_PROPERTY(long long id           READ id           WRITE setId           NOTIFY idChanged          )
    Q_PROPERTY(QDateTime dt           READ dt           WRITE setDt           NOTIFY dtChanged          )
    Q_PROPERTY(bool ativo             READ ativo        WRITE setAtivo        NOTIFY ativoChanged       )
    Q_PROPERTY(long long playlists_id READ playlists_id WRITE setPlaylists_id NOTIFY playlists_idChanged)
    Q_PROPERTY(QString name           READ name         WRITE setName         NOTIFY nameChanged        )
    Q_PROPERTY(QString uri            READ getUri       WRITE setUri          NOTIFY uriChanged         )
    Q_PROPERTY(QString artists        READ getArtists   WRITE setArtists      NOTIFY artistsChanged     )
    Q_PROPERTY(QString album_image    READ album_image  WRITE setAlbum_image  NOTIFY album_imageChanged )
    Q_PROPERTY(QString album          READ album        WRITE setAlbum        NOTIFY albumChanged       )
    Q_PROPERTY(QString url            READ url          WRITE setUrl          NOTIFY urlChanged         )
    Q_PROPERTY(QString preview_url    READ preview_url  WRITE setPreview_url  NOTIFY preview_urlChanged )
    Q_PROPERTY(QString duration       READ duration     WRITE setDuration     NOTIFY durationChanged    )

    Q_INVOKABLE explicit PlaylistMusic(QObject *parent = nullptr):QObject(parent){}
    ~PlaylistMusic(){}

    const QString &name() const;
    void setName(const QString &newName);

    const QString &getUri() const;
    void setUri(const QString &newUri);

    const QString &getArtists() const;
    void setArtists(const QString &newArtists);

    const QString &album_image() const;
    void setAlbum_image(const QString &newAlbum_image);

    const QString &album() const;
    void setAlbum(const QString &newAlbum);

    const QString &url() const;
    void setUrl(const QString &newUrl);

    const QString &preview_url() const;
    void setPreview_url(const QString &newPreview_url);

    const QString &duration() const;
    void setDuration(const QString &newDuration);

    long long id() const;
    void setId(long long newId);

    bool ativo() const;
    void setAtivo(bool newAtivo);

    const QDateTime &dt() const;
    void setDt(const QDateTime &newDt);

    long long playlists_id() const;
    void setPlaylists_id(long long newPlaylists_id);

signals:
    void nameChanged();

    void uriChanged();

    void artistsChanged();

    void album_imageChanged();

    void albumChanged();

    void urlChanged();

    void preview_urlChanged();

    void durationChanged();

    void idChanged();

    void ativoChanged();

    void dtChanged();

    void playlists_idChanged();

private:
    long long _id=0;
    long long _playlists_id=0;
    bool _ativo=true;
    QDateTime _dt=QDateTime::currentDateTime();
    QString _name="";
    QString _uri="";
    QString _album_image="";
    QString _album="";
    QString _url="";
    QString _preview_url="";
    QString _duration="";
    QString _artists="";
};

}
#endif // MODEL_PLAYLIST_MUSIC_H
