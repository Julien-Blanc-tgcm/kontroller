#include "utils.h"
#include "client.h"

QUrl getImageUrl(QString kodiUrl)
{
    if(kodiUrl.size() > 0)
    {
        QString url =QString("http://") + Client::current().serverAddress() + ":" + QString::number(Client::current().serverHttpPort()) + "/image/" +
                QUrl::toPercentEncoding(kodiUrl);
        return QUrl{url};
    }
    else
        return QUrl{};
}
