#include "db_field.h"


namespace db {

Field::Field(QString description, Field::Type type, bool primaryKey, bool autoincrement, QObject *parent) : QObject(parent),
    _description(description), _type(type), _primaryKey(primaryKey), _autoIncrement(autoincrement)
{
}

QString Field::description() const
{
    return _description;
}

void Field::setDescription(const QString &description)
{
    _description = description;
}

Field::Type Field::type() const
{
    return _type;
}

void Field::setType(const Type &type)
{
    _type = type;
}

bool Field::primaryKey() const
{
    return _primaryKey;
}

void Field::setPrimaryKey(bool primaryKey)
{
    _primaryKey = primaryKey;
}

void Field::setPrimaryKey(bool primaryKey, bool autoIncrement)
{
    _primaryKey = primaryKey;
    _autoIncrement = autoIncrement;
}

bool Field::autoIncrement() const
{
    return _autoIncrement;
}

void Field::setAutoIncrement(bool autoIncrement)
{
    _autoIncrement = autoIncrement;
}

QString Field::typeString(Field::Type type)
{
    QString text;

    switch (type) {
    case Field::INTEGER:
        text = "integer";
        break;
    case Field::REAL:
        text = "real";
        break;
    case Field::TEXT:
        text = "text";
        break;
    case Field::DATETIME:
        text = "datetime";
        break;
    case Field::BOOL:
        text = "integer";
        break;
    }

    return text;
}

QString Field::textToCreate()
{
    QString text = _description + " " + Field::typeString(_type);
    if(_primaryKey)
        text += " primary key";

    if(_autoIncrement)
        text += " autoincrement";

    return text;
}

}
