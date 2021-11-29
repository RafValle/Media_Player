#include "db_connection.h"

#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>


namespace db {

Connection &Connection::instance()
{
    static Connection __i;
    return __i;
}

void Connection::setup()
{
    this->setupDb();
}

QSqlDatabase Connection::db()
{
    return QSqlDatabase::database(QLatin1String("base"));
}

void Connection::setupDb()
{
    QString createdAppFolder = QString("%1/%2").arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) , QStringLiteral("db"));
    QDir appFolder(createdAppFolder);
    if (!appFolder.exists()){
        appFolder.mkpath(createdAppFolder);
    }
    if (appFolder.exists()){
        QString dbApp = QString("%1/%2.db").arg(createdAppFolder, QCoreApplication::applicationName().toLower());
        QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QLatin1String("base"));
        db.setDatabaseName(dbApp);
        if (!db.isValid()){
            qDebug() << "db connection is not valid";
        }
        if (!db.open()){
            qDebug() << "db connection is not opening";
        } else {
            db.close();
        }
    }
}

}

