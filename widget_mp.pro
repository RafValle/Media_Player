TARGET = WidgetMpApp
TEMPLATE = app

CONFIG += c++11

QT+= core gui widgets network

include ($$PWD/db/db.pri)
include ($$PWD/network/network.pri)
include ($$PWD/lib/lib.pri)

INCLUDEPATH += $$PWD/

SOURCES += \
    main.cpp \
    nova_playlist.cpp \
    playlists_window.cpp

HEADERS += \
    nova_playlist.h \
    playlists_window.h

FORMS += \
    nova_playlist.ui \
    playlists_window.ui
