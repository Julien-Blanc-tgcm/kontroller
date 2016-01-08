#include "server.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

Server::Server(QObject *parent) : QObject(parent),
    serverAddress_(""),
    serverPort_(9090),
    serverHttpPort_(8080)
{

}

}
}
}

