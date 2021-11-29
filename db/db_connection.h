#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <QObject>
#include <QSqlDatabase>

namespace db {

class Connection : public QObject
{
    Q_OBJECT
public:
    static Connection& instance();
    void setup();
    static QSqlDatabase db();
private:
    explicit Connection(QObject *parent = nullptr):QObject(parent){}
    void setupDb();
};

}
#endif // DB_MANAGER_H
