#include "utils.h"


namespace eu
{
namespace tgcm
{
namespace kontroller
{

QUrl getImageUrl(Client* client, QString kodiUrl)
{
	if(kodiUrl.size() > 0)
	{
		QString url = QString("http://") + client->serverAddress() + ":" + QString::number(client->serverHttpPort()) +
		              "/image/" +QUrl::toPercentEncoding(kodiUrl);
		return QUrl{url};
	}
	else
		return QUrl{};
}

}
}
}
