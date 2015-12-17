#include "utils.h"
#include "kodiclient.h"

QUrl getImageUrl(QString kodiUrl)
{
    if(kodiUrl.size() > 0)
    {
        QString url =QString("http://") + KodiClient::current().serverAddress() + ":" + QString::number(KodiClient::current().serverHttpPort()) + "/image/" +
                QUrl::toPercentEncoding(kodiUrl);
        return QUrl{url};
    }
    else
        return QUrl{};
}
