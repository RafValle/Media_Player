#ifndef DB_DAO_H
#define DB_DAO_H

#include "db_manager.h"

#include <QDebug>

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>

#include <QMetaProperty>
#include <QMetaObject>
#include <QMetaType>

namespace db {
    class Dao : public QObject
    {
        Q_OBJECT
    public:
        explicit Dao(QObject *parent = nullptr):QObject(parent){}
        ~Dao(){}

        template <class T>
        inline bool insert(QObject* object){
            QString insertStr = "INSERT INTO " + T::tableName() + " ";

            int propertyOffSet = T::staticMetaObject.propertyOffset();
            int propertyCount = T::staticMetaObject.propertyCount();

            QStringList properties;
            QStringList propertiesValues;

            for(int i = propertyOffSet; i < propertyCount; i++) {
                QByteArray propertyName = T::staticMetaObject.property(i).name();
                if(propertyName != T::pkField()){
                    properties << QString(propertyName);
                    propertiesValues << QString(":" + propertyName);
                }
            }

            insertStr += "(" + properties.join(", ") + ")";
            insertStr += "values (" + propertiesValues.join(", ") + ");";

            {
                this->newDb = Manager::getDatabase();
                if ((!this->newDb.isValid()) || (!this->newDb.open()))
                    qDebug() << this->newDb.lastError();
                else if(newDb.transaction()){

                    QSqlQuery query(this->newDb);
                    query.prepare(insertStr);
                    for(auto& propertyName: properties)
                         query.bindValue(":" + propertyName, object->property(propertyName.toUtf8()));

                    bool executed = query.exec();
                    if(executed) {
//                        query(, this->newDb);
                        if(query.exec("SELECT last_insert_rowid();")){
                            if(query.next()){
                                long long id = query.value(0).toLongLong();
                                object->setProperty(T::pkField().toUtf8(), id);
                                newDb.commit();
                                return true;
                            }
                        }
                    } else {
                        newDb.rollback();
                        qDebug() << query.lastError() << query.lastError().text();
                    }
                }
            }
//            Manager::removeDatabase(newDb.connectionName());
            return false;
        }

        template <class T>
        inline bool insertWithPK(QObject* object)
        {
            QString insertStr = "INSERT INTO " + T::tableName() + " ";

            int propertyOffSet = T::staticMetaObject.propertyOffset();
            int propertyCount = T::staticMetaObject.propertyCount();

            QStringList properties;
            QStringList propertiesValues;

            for(int i = propertyOffSet; i < propertyCount; i++){
                QByteArray propertyName = T::staticMetaObject.property(i).name();

                properties << QString(propertyName);
                propertiesValues << QString(":" + propertyName);
            }
            insertStr += "(" + properties.join(", ") + ")";
            insertStr += " values (" + propertiesValues.join(", ") + ");";

            if(newDb.transaction()) {
                QSqlQuery query(this->newDb);
                query.prepare(insertStr);

                for(auto&propertyName: properties)
                     query.bindValue(":" + propertyName, object->property(propertyName.toUtf8()));

                bool executed = query.exec();
                if(executed) {
                    newDb.commit();
                    return true;
                } else {
                    newDb.rollback();
                    qDebug() << query.lastError() << query.lastError().text();
                }
            }
            return false;
        }

        template <class T>
        inline T* selectFromWhere(QString whereClause = ""){
            T* t = NULL;

            QString select = "SELECT ";

            int propertyOffSet = T::staticMetaObject.propertyOffset();
            int propertyCount = T::staticMetaObject.propertyCount();

            QStringList properties;
            for(int i = propertyOffSet; i < propertyCount; i++) {
                QByteArray propertyName = T::staticMetaObject.property(i).name();
                properties << QString(propertyName);
            }

            select += properties.join(", ") + " FROM ";
            select += T::tableName() + " ";
            select += whereClause;

            QSqlQuery query(this->newDb);
            query.prepare(select);
            bool ok = query.exec();
            if(ok) {
                while(query.next()){
                    t = (T*) T::staticMetaObject.newInstance();
                    for(auto& property: properties)
                        t->setProperty(property.toUtf8(), query.value(property));
                }
            }
            return t;
        }

        template <class T>
        inline T* selectFromId(QVariant id){
            T* t = NULL;

            QString select = "SELECT ";

            int propertyOffSet = T::staticMetaObject.propertyOffset();
            int propertyCount = T::staticMetaObject.propertyCount();

            QStringList properties;
            for(int i = propertyOffSet; i < propertyCount; i++){
                QByteArray propertyName = T::staticMetaObject.property(i).name();
                properties << QString(propertyName);
            }
            select += properties.join(", ") + " FROM ";
            select += T::tableName() + " ";
            select += "WHERE " + T::pkField() + " = :ID";

            QSqlQuery query(this->newDb);
            query.prepare(select);
            query.bindValue(":ID", id);
            bool ok = query.exec();
            if(ok) {
                while(query.next()){
                    t = (T*) T::staticMetaObject.newInstance();
                    for(auto&property: properties)
                        t->setProperty(property.toUtf8(), query.value(property));
                }
            }
            return t;
        }

        template <class T>
        inline QList<T*> selectListFromWhere(QString whereClause = ""){
            QList<T*> objectsList;
            T* t = NULL;

            QString select = "SELECT ";
            int propertyOffSet = T::staticMetaObject.propertyOffset();
            int propertyCount = T::staticMetaObject.propertyCount();
            QStringList properties;
            for(int i = propertyOffSet; i < propertyCount; i++){
                QByteArray propertyName = T::staticMetaObject.property(i).name();
                properties << QString(propertyName);
            }
            select += properties.join(", ") + " FROM ";
            select += T::tableName() + " ";
            select += whereClause;

            QSqlQuery query(this->newDb);

            query.prepare(select);
            bool ok = query.exec();
            if(ok) {
                while(query.next()) {
                    t = (T*) T::staticMetaObject.newInstance();
                    for(auto &property: properties)
                        t->setProperty(property.toUtf8(), query.value(property));
                    objectsList << t;
                }
            }
            return objectsList;
        }

        template <class T>
        inline bool update(QObject* object, QString campo, QVariant valor){
            QSqlQuery query(this->newDb);
            QString sqlStr = "UPDATE " + T::tableName() + " SET " + campo + " = :VALOR WHERE " + T::pkField() + " = :" + T::pkField();
            qDebug() << sqlStr;
            query.prepare(sqlStr);
            query.bindValue(QString(":" + T::pkField()), object->property(T::pkField().toUtf8()));
            query.bindValue(":VALOR", valor);
            qDebug() << query.boundValues();
            bool ok = query.exec();
            return ok;
        }

        template <class T>
        inline bool update(QObject* object){
            QSqlQuery query(this->newDb);

            QString insertStr = "UPDATE " + T::tableName() + " SET ";

            int propertyOffSet = T::staticMetaObject.propertyOffset();
            int propertyCount = T::staticMetaObject.propertyCount();

            QStringList properties;
            QStringList propertiesNewValues;

            for(int i = propertyOffSet; i < propertyCount; i++) {
                QByteArray propertyName = T::staticMetaObject.property(i).name();
                properties << QString(propertyName);
                propertiesNewValues << QString(propertyName) + " = :" + QString(propertyName);
            }

            insertStr += propertiesNewValues.join(", ") + " ";
            insertStr +=  "WHERE " + T::pkField() + " = :" + T::pkField();
            query.prepare(insertStr);

            for(auto&propertyName: properties) {
                 query.bindValue(":" + propertyName, object->property(propertyName.toUtf8()));
            }
            qDebug() << insertStr;
            bool ok = query.exec();
            if(!ok){
                qDebug() << query.lastError().text();
            }
            return ok;
        }

        template <class T>
        inline bool update(QObject* object, QList<QByteArray> camposAtualizar, QString pkField = QString()){
            if(pkField.isEmpty()){
                pkField = T::pkField();
            }

            QSqlQuery query(this->newDb);

            QString insertStr = "UPDATE " + T::tableName() + " SET ";

            int propertyOffSet = T::staticMetaObject.propertyOffset();
            int propertyCount = T::staticMetaObject.propertyCount();

            QStringList properties;
            QStringList propertiesNewValues;

            for(int i = propertyOffSet; i < propertyCount; i++) {
                QByteArray propertyName = T::staticMetaObject.property(i).name();

                if(camposAtualizar.contains(propertyName) || propertyName == pkField) {
                    properties << QString(propertyName);
                    propertiesNewValues << QString(propertyName) + " = :" + QString(propertyName);
                }
            }
            insertStr += propertiesNewValues.join(", ") + " ";
            insertStr +=  "WHERE " + pkField + " = :" + pkField;
            query.prepare(insertStr);
            for(auto&propertyName: properties)
                 query.bindValue(":" + propertyName, object->property(propertyName.toUtf8()));

            qDebug() << insertStr;
            bool ok = query.exec();
            if(!ok){
                qDebug() << query.lastError().text();
            }
            return ok;
        }

        template <class T>
        inline bool deleteFromField(QObject* object, QString campo, QVariant valor){
            QSqlQuery query(this->newDb);
            QString sqlStr = "DELETE FROM " + T::tableName() + " WHERE " + campo + " = :VALOR";
            qDebug() << sqlStr;
            query.prepare(sqlStr);
            query.bindValue(QString(":" + T::pkField()), object->property(T::pkField().toUtf8()));
            query.bindValue(":VALOR", valor);
            qDebug() << query.boundValues();
            bool ok = query.exec();
            return ok;
        }

        template <class T>
        inline bool deleteAll(){
            QSqlQuery query(this->newDb);
            QString sqlStr = "DELETE FROM " + T::tableName();
            qDebug() << sqlStr;
            query.prepare(sqlStr);
            qDebug() << query.boundValues();
            bool ok = query.exec();
            return ok;
        }

        template <class T>
        inline int returnCount(QString whereClause = "") {
            QSqlQuery query(this->newDb);
            query.prepare("SELECT count(*) FROM " + T::tableName() + " " + whereClause);
            bool ok = query.exec();
            if(ok){
                while(query.next()) {
                    return query.value(0).toInt();
                }
            } else {
                return -1;
            }
        }

        template <class T>
        inline QVariantList returnValues(QString column, QString whereClause = "") {
            QVariantList RETURN;
            QSqlQuery query(this->newDb);
            query.prepare("SELECT "+column+" FROM " + T::tableName() + " " + whereClause);
            bool ok = query.exec();
            if(ok){
                while(query.next()){
                    RETURN << query.value(0);
                }
            } else {
                return RETURN;
            }
            return RETURN;
        }

        template <class T>
        inline QVariantList returnCountDistinct(QString column, QString whereClause = ""){

            QVariantList lista;
            QSqlQuery query(this->newDb);
            query.prepare("SELECT count(distinct "+column+") FROM " + T::tableName() + " " + whereClause);
            bool ok = query.exec();
            if(ok) {
                while(query.next())
                {
                    lista << query.value(0);
                }
            } else {
                return lista;
            }
            return lista;
        }

    signals:
        void error(QVariantMap &info);

    private:
        QSqlDatabase newDb;

    };
}
#endif // DB_MANAGER_H

