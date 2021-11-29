#include "playlists_window.h"
#include "ui_playlists_window.h"
#include "model_playlist_music.h"
#include "model_playlist.h"
#include "nova_playlist.h"
#include "db_dao.h"


#include "lib_requests.h"
#include "lib_manager.h"

#include <QDebug>

PlaylistsWindow::PlaylistsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlaylistsWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Playlists");
    this->resetSearch("");

    ui->twPlaylists->clear();
    ui->twPlaylists->setColumnCount(1);

    this->addItemLast();

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

    QTreeWidgetItem* playlist = ui->twPlaylists->currentItem();
    if (playlist == nullptr)
        return;


//    MVCGenericDAO<gm_fs_gestor::BloqueioRca> dao(this);
//        if(!dao.upsert(value)){
//            value->deleteLater();
//            return this->setLastErrorNull<BloqueioRca>();
//        }

//    {"isRegistered", false }

}


void PlaylistsWindow::addItemLast()
{
    QTreeWidgetItem* item =new QTreeWidgetItem();
    QLineEdit *lineEdit = new QLineEdit(dynamic_cast<QWidget*>(item));
    lineEdit->setText("New Playlist");
    item->setText(0, "New Playlist");
    item->setData(1, Qt::UserRole, QVariantMap{{"isRegistered", false }} );
    ui->twPlaylists->addTopLevelItem(item);
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

void PlaylistsWindow::mountTreeViewPlaylists(QVariantList playlists)
{
    Q_UNUSED(playlists)
    ui->twPlaylists->clear();
//    QTreeWidgetItem* item;
//    ItemTrack itemTrack;
//    itemTrack.readMap(playlist.toMap());
//    for(auto&playlist: playlists) {
//        item = new QTreeWidgetItem();
//        QLineEdit *lineEdit = new QLineEdit(dynamic_cast<QWidget*>(item));
//        lineEdit->setText(itemTrack);

////        item->setExpanded(
////        item->set

//    }
//        ui->twPlaylists->addTopLevelItem(item);
//        itemutil::setObject(itemDocumento, COLUNA_DESCRICAO, documento);
//    ui->twPlaylists->addTopLevelItem(item);
//     insertChildren(int index, const QList<QTreeWidgetItem *> &children)

//    playlists.isEmpty();
//        itemDocumento->setText(COLUNA_DESCRICAO, documento->descricao());
//        itemDocumento->setTextAlignment(COLUNA_DESCRICAO, Qt::AlignRight);

//        itemDocumento->setText(COLUNA_TIPO, documento->tipoDocumento()->toString());
//        itemDocumento->setTextAlignment(COLUNA_TIPO, Qt::AlignCenter);
//        itemDocumento->setText(COLUNA_DATA_INCLUSAO, documento->dataInclusao().toString("dd/MM/yyyy hh:mm:ss"));
//        itemDocumento->setTextAlignment(COLUNA_DATA_INCLUSAO, Qt::AlignCenter);
//    ui->twDocumentos->ajustarTamanhoColunasAConteudo();
}

//    disconnect(ui->tbSearch, &QTableWidget::itemClicked , this, &PlaylistsWindow::onTableItemClicked);
//    disconnect(ui->tbSearch->verticalScrollBar(), &QScrollBar::valueChanged , this, &PlaylistsWindow::onPesquisaDoNext);

//    QStringList item_counts({"Name", "Música", "Duration"});
//    ui->tbSearch->setColumnCount(item_counts.count());
//    ui->tbSearch->setHorizontalHeaderLabels(item_counts);

//    QTableWidgetItem* wgItem = nullptr;
//    int i=ui->tbSearch->rowCount();
//    for (auto&item:tracks){
//        ItemTrack track= this->getItem(item);
//        ui->tbSearch->insertRow(i);
//        wgItem = new QTableWidgetItem(track.name);
//        ui->tbSearch->setItem(i, 0, wgItem);
//        wgItem = new QTableWidgetItem(track.artists);
//        ui->tbSearch->setItem(i, 1, wgItem);
//        wgItem = new QTableWidgetItem(track.duration);
//        ui->tbSearch->setItem(i, 2, wgItem);
//        i++;
//    }
//    ui->tbSearch->resizeColumnsToContents();
//    ui->tbSearch->resizeColumnsToContents();
//    connect(ui->tbSearch, &QTableWidget::itemClicked , this, &PlaylistsWindow::onTableItemClicked);
//    connect(ui->tbSearch->verticalScrollBar(), &QScrollBar::valueChanged , this, &PlaylistsWindow::onPesquisaDoNext);
//wgItem->setData(0,track.getMap());
//QMessageBox::warning(NULL, "Aviso", msgCancelamento.arg(QString::number(registroAtual), QString::number(qtdRegistros)));
//WidgetUtil::mensagemAlerta("Não foi encontrado nenhum Comprovante para essa solicitação!");

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
    db::Dao dao(this);

}

