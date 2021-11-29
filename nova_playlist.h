#ifndef NOVA_PLAYLIST_H
#define NOVA_PLAYLIST_H

#include <QDialog>

namespace Ui {
class NovaPlaylist;
}

namespace model {
class Playlist;
}

class NovaPlaylist : public QDialog
{
    Q_OBJECT

public:
    explicit NovaPlaylist(QWidget *parent = nullptr);
    ~NovaPlaylist();

    bool inserirPlaylist(model::Playlist *value);
private slots:
    void on_btGravar_clicked();

    void on_btCancelar_clicked();

private:
    Ui::NovaPlaylist *ui;
};

#endif // NOVA_PLAYLIST_H
