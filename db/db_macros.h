#ifndef DB_MACROS_H
#define DB_MACROS_H

#include "db_table.h"
#include "db_field.h"


#define TABLE_FIELD(name)\
    public:\
    static QByteArray name##FIELD(){\
        return QByteArray(#name);\
    }\

#define PK_FIELD(name)\
    public:\
    static QString pkField(){\
        return QString(#name);\
    }\

#define TABLE_NAME(name)\
    public:\
    static QString tableName()\
    {\
        return QString(#name);\
    }\

#endif // DB_MACROS_H

