#include "db_table.h"

namespace db {

Table::Table(const QString&description, const QString& schema, QObject *parent) : QObject(parent), _schema(schema), _description(description)
{
}

QString Table::schema() const
{
    return _schema;
}

void Table::setSchema(const QString &schema)
{
    _schema = schema.simplified();
}

QString Table::description() const
{
    return _description;
}

void Table::setDescription(const QString &description)
{
    _description = description.simplified();
}

QList<Field *> Table::fields() const
{
    return _fields;
}

void Table::setFields(const QList<Field *> &fields)
{
    _fields = fields;
}

void Table::addField(Field *field)
{
    _fields << field;
}

QString Table::textToCreate()
{
    QString createString = QStringLiteral("CREATE TABLE ");

    if(!_schema.isEmpty())
        createString.append(QStringLiteral("%1.").arg(_schema));
    createString.append(QStringLiteral("%1 ").arg(_description));

    QStringList fields;
    for(auto &item: _fields)
        fields << item->textToCreate();

    if (!fields.isEmpty())
        createString.append(QStringLiteral("(%1)").arg(fields.join(", ")));

    if (!createString.isEmpty())
        createString.append(";");

    return createString;
}

}
