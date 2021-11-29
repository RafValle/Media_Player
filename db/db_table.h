#ifndef DB_TABLE_H
#define DB_TABLE_H

#include <QObject>

#include "db_field.h"

namespace db {

class Table : public QObject
{
    Q_OBJECT
public:
    explicit Table(const QString&description, const QString& schema = "", QObject *parent = nullptr);

    QString schema() const;
    void setSchema(const QString &schema);

    QString description() const;
    void setDescription(const QString &description);

    QList<Field *> fields() const;

    void setFields(const QList<Field*> &fields);
    void addField(Field* field);

    QString textToCreate();

private:
    QString _schema;
    QString _description;
    QList<Field*> _fields;
};

}

#endif // DB_TABLE_H
