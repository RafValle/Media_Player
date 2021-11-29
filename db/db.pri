QT += core sql
INCLUDEPATH += $$PWD/
HEADERS += \
        $$PWD/db_connection.h \
        $$PWD/db_dao.h \
        $$PWD/db_field.h \
        $$PWD/db_macros.h \
        $$PWD/db_table.h \
        $$PWD/db_result.h \
        $$PWD/db_manager.h

SOURCES += \
        $$PWD/db_connection.cpp \
        $$PWD/db_dao.cpp \
        $$PWD/db_field.cpp \
        $$PWD/db_table.cpp \
        $$PWD/db_result.cpp \
        $$PWD/db_manager.cpp

INCLUDEPATH += $$PWD
