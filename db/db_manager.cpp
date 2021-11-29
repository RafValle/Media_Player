#include "db_manager.h"
#include "db_connection.h"

#include <QCoreApplication>
#include <QUuid>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QString>
#include <QDebug>

namespace db {

QString checkStmtTable(const QString&table) {
    return QStringLiteral("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name='%1';").arg(table);
}

QString checkStmtTableField(const QString&table, const QString&field) {
    return QStringLiteral("SELECT COUNT(*) FROM pragma_table_info('%1') WHERE name='%2'").arg(table, field);
}

QString alterStmtTableField(const QString&table, const QString&field) {
    return QStringLiteral("ALTER TABLE %1 ADD COLUMN %2").arg(table, field);
}

bool exec(QSqlQuery&queryExec, QString &stmt) {
    if (queryExec.prepare(stmt))
        if (queryExec.exec())
            return true;

    qDebug() << queryExec.lastQuery();
    qDebug() << queryExec.lastError();
    return false;
}

bool createTable(QSqlDatabase&newDb, Table* table){
    QSqlQuery queryExec(newDb);
    QString create = table->textToCreate();
    if (!exec(queryExec, create)){
        return false;
    }
    return true;
}

bool checkTable(QSqlDatabase&newDb, Table* table){
    bool hasTable = false;
    QSqlQuery queryExec(newDb);
    auto check = checkStmtTable(table->description());
    if (!exec(queryExec, check)){
        return false;
    } else if (queryExec.next()){
        int qtdTable = queryExec.value(0).toInt();
        hasTable = (qtdTable > 0);
    }
    if (!hasTable)
        return createTable(newDb, table);
    return hasTable;
}

bool createFields(QSqlDatabase&newDb, Table* table, QList<Field*> fields){
    QSqlQuery queryExec(newDb);
    QListIterator<Field*> i(fields);
    while(i.hasNext()){
        auto field=i.next();
        auto createField = alterStmtTableField(table->description(), field->textToCreate());
        if (!exec(queryExec, createField))
            return false;
    }
    return true;
}

bool checktField(QSqlDatabase&newDb, Table* table){
    QSqlQuery queryExec(newDb);

    QList<Field*> pendingFields;
    QListIterator<Field*> i(table->fields());
    while(i.hasNext()){
        auto field=i.next();
        auto checkField = checkStmtTableField(table->description(), field->description());
        if (!exec(queryExec, checkField)){
            return false;
        } else if (queryExec.next()){
            int qtdField = queryExec.value(0).toInt();
            bool hasField = ( qtdField > 0);
            if (!hasField){
                pendingFields << field;
            }
        }
    }
    if (!pendingFields.isEmpty()){
        return createFields(newDb, table, pendingFields);
    }
    return true;
}

QSqlDatabase Manager::getDatabase()
{
    QSqlDatabase db = db::Connection::instance().db();
    if (!db.isValid())
        return QSqlDatabase();
    auto connectionName = QUuid::createUuid().toString();
    auto newDb = QSqlDatabase::cloneDatabase(db, connectionName);
    return newDb;
}

void Manager::removeDatabase(const QString connectionName)
{
    QSqlDatabase::removeDatabase(connectionName);
}

void Manager::setup()
{
    auto newDb = this->getDatabase();
    if (!newDb.isValid())
        return;

    if (!newDb.open())
        return;

    QList<Table*> tables = this->tables();
    QListIterator<Table*> i(tables);
    while(i.hasNext()){
        auto table=i.next();
        if (checkTable(newDb,table)){
            checktField(newDb,table);
        }
    }
}

}
