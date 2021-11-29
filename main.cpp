#include <QApplication>
#include "playlists_window.h"

#include "lib_requests.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlaylistsWindow w(nullptr);
    w.show();
    return a.exec();
}
