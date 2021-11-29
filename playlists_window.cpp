#include "playlists_window.h"
#include "ui_playlists_window.h"
#include "model_playlist_music.h"
#include "model_playlist.h"
#include "nova_playlist.h"
#include "db_dao.h"
#include "db_manager.h"
#include "db_connection.h"

#include "lib_requests.h"
#include "lib_manager.h"
#include <QSqlQuery>
#include <QDebug>

PlaylistsWindow::PlaylistsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlaylistsWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Playlists");
    this->resetSearch("");

//    ui->tbPlaylists->clear();
//    ui->tbPlaylists->setColumnCount(1);

    this->getPlayList();

    ui->leSearch->setFocus();

    this->clearProgress();

    ui->tbSearch->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connect(ui->leSearch, &QLineEdit::returnPressed , this, &PlaylistsWindow::onPesquisaDo );
    connect(ui->pbSearch, &QPushButton::clicked , this, &PlaylistsWindow::onPesquisaDo );
    connect(ui->pbAdd, &QPushButton::clicked , this, &PlaylistsWindow::onAddMusic );
}

PlaylistsWindow::~PlaylistsWindow()
{
    delete ui;
}

void PlaylistsWindow::addMusic()
{

}

void PlaylistsWindow::onAddMusic()
{
    QList<QTableWidgetItem*> tracks = ui->tbSearch->selectedItems();

    if (tracks.isEmpty())
        return;
     QList<model::PlaylistMusic*> listMusic;

    foreach(QTableWidgetItem* item, tracks){
        model::PlaylistMusic* music = this->getObject<model::PlaylistMusic>(item);
            music->setPlaylists_id(ui->tbPlaylists->currentItem()->text().toLongLong());
            listMusic << music;

    }

//    QTreeWidgetItem* playlist = ui->twPlaylists->currentItem();
//    if (playlist == nullptr)
//        return;

}

void PlaylistsWindow::savePlaylist(QVariantMap playlist)
{
    Q_UNUSED(playlist)
}

void PlaylistsWindow::onPesquisaDo()
{
    this->disableSearch();
    this->searchResultsBy(ui->leSearch->text());
    this->enableSearch();
}

void PlaylistsWindow::onPesquisaDoNext(int value)
{
    if (ui->tbSearch->verticalScrollBar()->maximum() == value){
        this->onPesquisaDo();
    }
}

void PlaylistsWindow::onTableItemClicked(QTableWidgetItem *item)
{
   Q_UNUSED(item)
   return;
}

void PlaylistsWindow::addPlaylist(QVariant response)
{

    _tracks.insert(_tracks.size(), response);
    QVariantMap listMap = response.toMap().value(QByteArrayLiteral("tracks")).toMap();
    if (listMap.isEmpty())
        return;
    auto items = listMap.value(QByteArrayLiteral("items")).toList();
    auto totalItems = listMap.value(QByteArrayLiteral("total")).toInt();
    this->_total=totalItems;
    this->mountTableMusics(items);
}

void PlaylistsWindow::addMusics(QVariant response)
{
    _tracks.insert(_tracks.size(), response);
    QVariantMap trackMap = response.toMap().value(QByteArrayLiteral("tracks")).toMap();
    if (trackMap.isEmpty())
        return;
    auto items = trackMap.value(QByteArrayLiteral("items")).toList();
    auto totalItems = trackMap.value(QByteArrayLiteral("total")).toInt();
    this->_total=totalItems;
    this->mountTableMusics(items);
}

void PlaylistsWindow::mountTableMusics(QVariantList tracks)
{
    disconnect(ui->tbSearch, &QTableWidget::itemClicked , this, &PlaylistsWindow::onTableItemClicked);
    disconnect(ui->tbSearch->verticalScrollBar(), &QScrollBar::valueChanged , this, &PlaylistsWindow::onPesquisaDoNext);

    QStringList item_counts({"Music", "Artist", "Duration"});
    ui->tbSearch->setColumnCount(item_counts.count());
    ui->tbSearch->setHorizontalHeaderLabels(item_counts);

    QTableWidgetItem* wgItem = nullptr;
    int i=ui->tbSearch->rowCount();
    for (auto&item:tracks){
        ItemTrack track= this->getItem(item);
        ui->tbSearch->insertRow(i);
        wgItem = new QTableWidgetItem(track.name);
        wgItem->setData(3,track.getMap());
        ui->tbSearch->setItem(i, 0, wgItem);
        wgItem = new QTableWidgetItem(track.artists);
        ui->tbSearch->setItem(i, 1, wgItem);
        wgItem = new QTableWidgetItem(track.duration);
        ui->tbSearch->setItem(i, 2, wgItem);
        i++;
    }
    ui->tbSearch->resizeRowsToContents();
    ui->tbSearch->resizeColumnsToContents();

    connect(ui->tbSearch, &QTableWidget::itemClicked , this, &PlaylistsWindow::onTableItemClicked);
    connect(ui->tbSearch->verticalScrollBar(), &QScrollBar::valueChanged , this, &PlaylistsWindow::onPesquisaDoNext);
}

void PlaylistsWindow::mountTableViewPlaylists(QVariantList playlists)
{
    Q_UNUSED(playlists)
//    ui->twPlaylists->clear();

    disconnect(ui->tbSearch, &QTableWidget::itemClicked , this, &PlaylistsWindow::onTableItemClicked);
    disconnect(ui->tbSearch->verticalScrollBar(), &QScrollBar::valueChanged , this, &PlaylistsWindow::onPesquisaDoNext);

    QStringList item_counts({"Playlist"});
    ui->tbSearch->setColumnCount(item_counts.count());
    ui->tbSearch->setHorizontalHeaderLabels(item_counts);

    QTableWidgetItem* wgItem = nullptr;
    int i=ui->tbSearch->rowCount();
    for (auto&item:playlists){
        ItemTrack play= this->getItem(item);
        ui->tbSearch->insertRow(i);
        wgItem = new QTableWidgetItem(play.name);
        wgItem->setData(3,play.getMap());
        i++;
    }
    ui->tbSearch->resizeRowsToContents();
    ui->tbSearch->resizeColumnsToContents();

    connect(ui->tbSearch, &QTableWidget::itemClicked , this, &PlaylistsWindow::onTableItemClicked);
    connect(ui->tbSearch->verticalScrollBar(), &QScrollBar::valueChanged , this, &PlaylistsWindow::onPesquisaDoNext);
}


void PlaylistsWindow::searchResultsBy(const QString &term)
{
    this->fullProgress();
    auto termEdited =term.toLower().simplified();
    if (termEdited.isEmpty())
        return;

    if (_lasterm.compare(termEdited, Qt::CaseInsensitive) !=0 ){
        this->resetSearch(termEdited);
    } else {
        auto newOffset = ui->tbSearch->rowCount();
        if ((newOffset >=this->_total) && (this->_total!=0)){
            qDebug() << "chegou ao máximo dessa pesquisa";
            this->clearProgress();
            return;
        } else {
            this->_offset = newOffset;
        }
    }
    auto tracks = lib::Requests::instance().search(this->_lasterm, this->_offset);
    this->addMusics(tracks);
    this->clearProgress();
}

void PlaylistsWindow::searchPlaylists()
{
    this->fullProgress();
    auto tracks = lib::Requests::instance().search(this->_lasterm, this->_offset);
    this->clearProgress();
}

void PlaylistsWindow::resetSearch(const QString search)
{
    ui->tbSearch->clear();
    this->_lasterm =search;
    this->_offset = 0;
    this->_limit = 0;
    this->_total  = 0;
}

void PlaylistsWindow::enableSearch()
{
    ui->leSearch->setEnabled(true);
    ui->pbSearch->setEnabled(true);
    ui->pbAdd->setEnabled(true);
}

void PlaylistsWindow::disableSearch()
{
    ui->leSearch->setEnabled(false);
    ui->pbSearch->setEnabled(false);
    ui->pbAdd->setEnabled(false);
}

void PlaylistsWindow::fullProgress()
{
    ui->progressSearch->setMinimum(0);
    ui->progressSearch->setMaximum(0);
}

void PlaylistsWindow::clearProgress()
{
    ui->progressSearch->setMinimum(100);
    ui->progressSearch->setMaximum(100);
}

ItemTrack PlaylistsWindow::getItem(QVariant dados)
{
    ItemTrack item;

    QVariantMap dadosMap = dados.toMap();
    QVariantMap albumMap = dadosMap.value("album").toMap();
    QVariantList albumImages = albumMap.value("images").toList();

    QVariantList artistas = dadosMap.value("artists").toList();

    item.album = albumMap.value("name").toString();
    item.album_image = albumImages.isEmpty()?"":albumImages.first().toMap().value("url").toString();
    item.url = dadosMap.value("external_urls").toMap().value("spotify").toString();
    item.preview_url = albumMap.value("preview_url").toString();
    item.name = dadosMap.value("name").toString();

    QStringList artistasName;
    for(auto&artista: artistas)
        artistasName << artista.toMap().value("name").toString();
    item.artists = artistasName.join(',');

    auto duration_ms = dadosMap.value("duration_ms").toInt();
    duration_ms = duration_ms * 0.001;
    auto m = (duration_ms/60);
    auto s = (duration_ms%60);

    item.duration = QString::number(m)+":"+QString::number(s).rightJustified(2,'0');
    item.uri = dadosMap.value("uri").toString();

    return item;
}

void PlaylistsWindow::on_pushButton_clicked()
{
    NovaPlaylist novaPlaylist;
    novaPlaylist.exec();
}

void PlaylistsWindow::getPlayList()
{
    QString select = "select * from playlist";

    QSqlQuery query(db::Connection::db());
    query.prepare(select);
    bool ok = query.exec();
    if(ok){
        int count = 0;
        ui->tbPlaylists->setColumnCount(2);
        while (query.next()) {
            ui->tbPlaylists->insertRow(count);
            ui->tbPlaylists->setItem(count,0, new QTableWidgetItem(query.value(3).toString()));
            ui->tbPlaylists->setItem(count,1, new QTableWidgetItem(query.value(1).toString()));
            ui->tbPlaylists->setRowHeight(count,40);
            count++;
        }

        ui->tbPlaylists->setColumnWidth(0,300);
        ui->tbPlaylists->setColumnWidth(1,150);

    }else{
        QMessageBox::warning(this,"ERRO","Erro ao pesquisar playlists");

    }


}

