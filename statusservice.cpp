#include "statusservice.h"
#include "kodiclient.h"

StatusService::StatusService(QObject* parent) :
    QObject(parent)
{
    connect(&KodiClient::current(), SIGNAL(connectionStatusChanged(int)), this, SLOT(updateConnectionStatus(int)));
}

int StatusService::connectionStatus() const
{
    return KodiClient::current().connectionStatus();
}

void StatusService::updateConnectionStatus(int connectionStatus)
{
    emit connectionStatusChanged(connectionStatus);
}

