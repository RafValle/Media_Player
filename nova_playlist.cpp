#include "nova_playlist.h"
#include "ui_nova_playlist.h"
#include <QtSql>
#include <QMessageBox>
#include "model_playlist.h"
#include "db_dao.h"


NovaPlaylist::NovaPlaylist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NovaPlaylist)
{
    ui->setupUi(this);
}

NovaPlaylist::~NovaPlaylist()
{
    delete ui;
}


void NovaPlaylist::on_btGravar_clicked()
{
    QString nome = ui->leNamePlaylist->text();

    model::Playlist * model = new model::Playlist();

    model->setName(nome);

    if(!this->inserirPlaylist(model)){
        QMessageBox::information(this,"","Erro ao criar playlist");
    }else{
        QMessageBox::information(this,"","Playlist salva com sucesso");
        ui->leNamePlaylist->clear();
        ui->leNamePlaylist->setFocus();
    }

    //CREATE TABLE playlist (id integer primary key autoincrement, dt datetime, ativo integer, name text)
}


bool NovaPlaylist::inserirPlaylist(model::Playlist *value)
{
    qDebug()<<__PRETTY_FUNCTION__;
    if (!value)
        qDebug() << "sem dados";

    db::Dao dao(this);
    if (!dao.insert<model::Playlist>(value)){
        return false;
    }
    return true;
}

void NovaPlaylist::on_btCancelar_clicked()
{

}

