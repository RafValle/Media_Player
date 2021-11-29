#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "db_table.h"
#include "db_result.h"

namespace db {

class Manager : public QObject
{
    Q_OBJECT

public:
    explicit Manager(QObject *parent = nullptr):QObject(parent){}
    ~Manager(){}

    void setup();

    static QSqlDatabase getDatabase();
    static void removeDatabase(const QString connectionName);

protected:
    virtual QList<Table*> tables() = 0;
    virtual void preSetup(const QString&path) = 0;

signals:
    void error(QVariantMap &info);    
};

}
#endif // DB_MANAGER_H
