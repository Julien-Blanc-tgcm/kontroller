#ifndef EU_TGCM_KONTROLLER_UTILS_H
#define EU_TGCM_KONTROLLER_UTILS_H

#include "client.h"

#include <QString>
#include <QUrl>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

QUrl getImageUrl(Client* client, QString kodiUrl);

}
}
}

#endif // EU_TGCM_KONTROLLER_UTILS_H

