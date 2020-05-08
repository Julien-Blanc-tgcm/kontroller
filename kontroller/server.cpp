#include "server.h"

#include <QUuid>

#include <kodivolumeplugin.h>
#include <minidspvolumeplugin.h>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

Server::Server(QObject *parent) : QObject(parent),
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

QString Server::login() const
{
	return login_;
}

QString Server::password() const
{
	return password_;
}

QString Server::volumePluginName() const
{
	return volumePluginName_;
}

QVariantMap const& Server::volumePluginParameters() const
{
	return volumePluginParameters_;
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

void Server::setLogin(QString login)
{
	if (login_ == login)
		return;

	login_ = login;
	emit loginChanged(login_);
}

void Server::setPassword(QString password)
{
	if (password_ == password)
		return;

	password_ = password;
	emit passwordChanged(password_);
}

void Server::setVolumePluginName(QString volumePluginName)
{
	if(volumePluginName_ == volumePluginName)
		return;
	volumePluginName_ = volumePluginName;
	emit volumePluginNameChanged(volumePluginName_);
}

void Server::setVolumePluginParameters(QVariantMap parameters)
{
	volumePluginParameters_ = parameters;
	emit volumePluginParametersChanged();
}

}
}
}

