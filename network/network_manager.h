#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QObject>
#include <QVariantMap>

#include "network_request.h"

namespace network {

class Manager : public QObject
{
    Q_OBJECT
public:
    static Manager& instance();
    bool call(Request &request);
protected:
    explicit Manager(QObject *parent = nullptr):QObject(parent){}
    ~Manager(){}
signals:
    void error(QVariantMap &info);    
};

}
#endif // NETWORK_MANAGER_H
