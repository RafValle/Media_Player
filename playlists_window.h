#ifndef PlaylistsWindow_H
#define PlaylistsWindow_H

#include <QMutex>
#include <QMainWindow>
#include <QVariantMap>
#include <QScrollBar>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class PlaylistsWindow; }

namespace model {
class Playlist;
}

QT_END_NAMESPACE


typedef struct ItemTrack {
    QString uri;
    QString album_image;
    QString album;
    QString url;
    QString preview_url;
    QString duration;
    QString name;
    QString artists;

    QVariantMap getMap(){
        QVariantMap map;
        map.insert("uri"         , uri        );
        map.insert("album_image" , album_image);
        map.insert("album"       , album      );
        map.insert("url"         , url        );
        map.insert("preview_url" , preview_url);
        map.insert("duration"    , duration   );
        map.insert("name"        , name       );
        map.insert("artists"     , artists    );
        return map;
    }

    void readMap(QVariantMap map){
        uri         = map.value("uri"        ).toString();
        album_image = map.value("album_image").toString();
        album       = map.value("album"      ).toString();
        url         = map.value("url"        ).toString();
        preview_url = map.value("preview_url").toString();
        duration    = map.value("duration"   ).toString();
        name        = map.value("name"       ).toString();
        artists     = map.value("artists"    ).toString();
    }

} ItemTrack;

class PlaylistsWindow : public QMainWindow
{
    Q_OBJECT

public:
    PlaylistsWindow(QWidget *parent = nullptr);
    ~PlaylistsWindow();

public slots:
    void addMusic();

    void onAddMusic();

    void savePlaylist(QVariantMap playlist);

    void onPesquisaDo();
    void onPesquisaDoNext(int value);

    void onTableItemClicked(QTableWidgetItem* item);

    void addMusics(QVariant response);
    void mountTableMusics(QVariantList tracks);

    void mountTreeViewPlaylists(QVariantList playlists);

    void getPlayList();
private slots:
    void on_pushButton_clicked();

private:
    void addItemLast();

    void searchResultsBy(const QString &term);

    void searchPlaylists();

    void resetSearch(const QString search);

    void enableSearch();
    void disableSearch();

    void fullProgress();
    void clearProgress();

    ItemTrack getItem(QVariant dados);

private:
    Ui::PlaylistsWindow *ui;

    QVariantList _tracks;
    QString _lasterm ="";

    int _total = 0;
    int _offset = 0;
    int _limit = 0;
};
#endif // PlaylistsWindow_H
