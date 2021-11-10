#include "server.h"

#include <QUuid>

namespace eu ::tgcm ::kontroller
{

bool Server::ignoreWifiStatus() const
{
	return ignoreWifi_;
}

void Server::setIgnoreWifiStatus(bool newIgnoreWifi)
{
	if (ignoreWifi_ == newIgnoreWifi)
		return;
	ignoreWifi_ = newIgnoreWifi;
	emit ignoreWifiStatusChanged();
}

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

QString Server::wakeUpPluginName() const
{
	return wakeUpPluginName_;
}

const QVariantMap& Server::wakeUpPluginParameters() const
{
	return wakeUpPluginParameters_;
}

bool Server::suspendEnabled() const
{
	return suspendEnabled_;
}

bool Server::hibernateEnabled() const
{
	return hibernateEnabled_;
}

bool Server::rebootEnabled() const
{
	return rebootEnabled_;
}

bool Server::poweroffEnabled() const
{
	return poweroffEnabled_;
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

void Server::setWakeUpPluginName(QString wakeUpPluginName)
{
	if (wakeUpPluginName_ == wakeUpPluginName)
		return;

	wakeUpPluginName_ = wakeUpPluginName;
	emit wakeUpPluginNameChanged(wakeUpPluginName_);
}

void Server::setWakeUpPluginParameters(QVariantMap wakeUpPluginParameters)
{
	if (wakeUpPluginParameters_ == wakeUpPluginParameters)
		return;

	wakeUpPluginParameters_ = wakeUpPluginParameters;
	emit wakeUpPluginParametersChanged(wakeUpPluginParameters_);
}

void Server::setSuspendEnabled(bool suspendEnabled)
{
	if (suspendEnabled_ == suspendEnabled)
		return;

	suspendEnabled_ = suspendEnabled;
	emit suspendEnabledChanged(suspendEnabled_);
}

void Server::setHibernateEnabled(bool hibernateEnabled)
{
	if (hibernateEnabled_ == hibernateEnabled)
		return;

	hibernateEnabled_ = hibernateEnabled;
	emit hibernateEnabledChanged(hibernateEnabled_);
}

void Server::setRebootEnabled(bool rebootEnabled)
{
	if (rebootEnabled_ == rebootEnabled)
		return;

	rebootEnabled_ = rebootEnabled;
	emit rebootEnabledChanged(rebootEnabled_);
}

void Server::setPoweroffEnabled(bool poweroffEnabled)
{
	if (poweroffEnabled_ == poweroffEnabled)
		return;

	poweroffEnabled_ = poweroffEnabled;
	emit poweroffEnabledChanged(poweroffEnabled_);
}

} // namespace eu::tgcm::kontroller
