#include "lib_manager.h"

#include "db_connection.h"
#include "db_manager.h"
#include "db_dao.h"

#include "model_requests.h"
#include "model_playlist.h"

static void init(){
    db::Connection::instance().setup();
    lib::Manager::instance().setup();
}
Q_COREAPP_STARTUP_FUNCTION(init);

namespace lib {

    lib::Manager &Manager::instance()
    {
        static Manager __i;
        return __i;
    }

    QList<db::Table *> Manager::tables()
    {
        QList<db::Table*> tables;
        db::Table * table = new db::Table("requests");
        table->addField(new db::Field("id", db::Field::INTEGER, true, true));
        table->addField(new db::Field("dt", db::Field::DATETIME));
        table->addField(new db::Field("ativo", db::Field::BOOL));
        table->addField(new db::Field("status", db::Field::INTEGER));
        table->addField(new db::Field("body", db::Field::TEXT));
        table->addField(new db::Field("url", db::Field::TEXT));
        tables << table;

        table = new db::Table("playlist");
        table->addField(new db::Field("id", db::Field::INTEGER, true, true));
        table->addField(new db::Field("dt", db::Field::DATETIME));
        table->addField(new db::Field("ativo", db::Field::BOOL));
        table->addField(new db::Field("name", db::Field::TEXT));
        tables << table;

        table = new db::Table("playlist_music");
        table->addField(new db::Field("id", db::Field::INTEGER, true, true));
        table->addField(new db::Field("playlists_id", db::Field::INTEGER));
        table->addField(new db::Field("dt", db::Field::DATETIME));
        table->addField(new db::Field("ativo", db::Field::BOOL));
        table->addField(new db::Field("name", db::Field::TEXT));
        table->addField(new db::Field("uri", db::Field::TEXT));
        table->addField(new db::Field("artists", db::Field::TEXT));
        table->addField(new db::Field("album_image", db::Field::TEXT));
        table->addField(new db::Field("album", db::Field::TEXT));
        table->addField(new db::Field("url", db::Field::TEXT));
        table->addField(new db::Field("preview_url", db::Field::TEXT));
        table->addField(new db::Field("duration", db::Field::TEXT));
        tables << table;

        return tables;
    }

    void Manager::preSetup(const QString&path)
    {
        Q_UNUSED(path)
    }

    bool Manager::saveRequest(int code, const QByteArray body, const QByteArray url)
    {
        model::Requests response;
        response.setStatus(code);
        response.setBody(body);
        response.setUrl(url);
        db::Dao dao(this);
        if (!dao.insert<model::Requests>(&response)){
            return false;
        }
        return true;
    }

    QVariantList Manager::getPlaylist()
    {
        QVariantList playlistList;
        db::Dao dao(this);
        QList<model::Playlist*> playlists = dao.selectListFromWhere<model::Playlist>();
        for (auto&item:playlists){
            QVariantMap itemMap;
            itemMap.insert("id"   , item->id());
            itemMap.insert("dt"   , item->dt());
            itemMap.insert("ativo", item->ativo());
            itemMap.insert("name" , item->name());
            playlistList << itemMap;
        }
        return playlistList;
    }

    QVariantMap Manager::createPlaylist(QString name)
    {
        return QVariantMap();
    }
}
