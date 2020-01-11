#include "server.h"

#include <QUuid>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

Server::Server(QObject *parent) : QObject(parent),
    serverAddress_(""),
    serverPort_(9090),
    serverHttpPort_(8080),
    hasZones_(false),
    uuid_(QUuid::createUuid().toString())
{

}

QString Server::serverAddress() const
{
	return serverAddress_;
}

int Server::serverPort() const
{
	return serverPort_;
}

int Server::serverHttpPort() const
{
	return serverHttpPort_;
}

QString Server::name() const
{
	return name_;
}

bool Server::hasZones() const
{
	return hasZones_;
}

QStringList Server::zones() const
{
	return zones_;
}

QString Server::uuid() const
{
	return uuid_;
}

void Server::setServerAddress(QString serverAddress)
{
	if (serverAddress_ == serverAddress)
		return;

	serverAddress_ = serverAddress;
	emit serverAddressChanged(serverAddress);
}

void Server::setServerPort(int serverPort)
{
	if (serverPort_ == serverPort)
		return;

	serverPort_ = serverPort;
	emit serverPortChanged(serverPort);
}

void Server::setServerHttpPort(int serverHttpPort)
{
	if (serverHttpPort_ == serverHttpPort)
		return;

	serverHttpPort_ = serverHttpPort;
	emit serverHttpPortChanged(serverHttpPort);
}

void Server::setName(QString name)
{
	if (name_ == name)
		return;

	name_ = name;
	emit nameChanged(name);
}

void Server::setHasZones(bool hasZones)
{
	if(hasZones == hasZones_)
		return;
	hasZones_ = hasZones;
	emit hasZonesChanged(hasZones);
}

void Server::setZones(QStringList zones)
{
	if(zones == zones_)
		return;
	zones_ = zones;
	emit zonesChanged(zones_);
}

void Server::setUuid(QString uuid)
{
	if (uuid_ == uuid)
		return;

	uuid_ = uuid;
	emit uuidChanged(uuid_);
}

}
}
}

