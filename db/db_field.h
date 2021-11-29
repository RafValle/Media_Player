#ifndef DB_FIELD_H
#define DB_FIELD_H

#include <QObject>

namespace db {

    class Field : public QObject
    {
        Q_OBJECT
    public:

        enum Type {
            INTEGER ,
            REAL    ,
            TEXT    ,
            DATETIME,
            BOOL
        };

        Field(QString description, Type type, bool primaryKey = false, bool autoincrement = false, QObject *parent = nullptr);

        QString description() const;
        void setDescription(const QString &description);

        Type type() const;
        void setType(const Type &type);

        bool primaryKey() const;
        void setPrimaryKey(bool primaryKey);
        void setPrimaryKey(bool primaryKey, bool autoIncrement);

        bool autoIncrement() const;
        void setAutoIncrement(bool autoIncrement);

        static QString typeString(Type type);

        QString textToCreate();

    private:
        Type _type;
        QString _description="";
        bool _primaryKey;
        bool _autoIncrement;
    };
}
#endif // SQLITEFIELD_H
