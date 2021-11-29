#include "network_manager.h"
#include <QNetworkReply>

namespace network {

Manager &Manager::instance()
{
    static Manager __i;
    return __i;
}

bool Manager::call(Request&request)
{
   RequestJob thread(request);
   thread.start();
   emit thread.started();
   thread.wait();
   thread.deleteLater();
   return (request.response.qtCode == QNetworkReply::NoError);
}

}
