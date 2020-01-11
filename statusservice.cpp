#include "statusservice.h"
#include "client.h"
#include <QNetworkConfigurationManager>
#include "settings.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

StatusService::StatusService(QObject* parent) :
    QObject(parent)
{
	connect(&Client::current(), SIGNAL(connectionStatusChanged(int)), this, SLOT(updateConnectionStatus(int)));
	connect(&Client::current(), &Client::serverChanged, this, &StatusService::serverChanged);
	connect(&Client::current(), &Client::serverChanged, this, &StatusService::serverNameChanged);
	setWifiEnabled(atLeastOneWifiConnected());
	connect(&manager_, &QNetworkConfigurationManager::configurationAdded, this, &StatusService::handleConnectionAdded_);
	connect(&manager_, &QNetworkConfigurationManager::configurationRemoved, this, &StatusService::handleConnectionRemoved_);
	connect(&manager_, &QNetworkConfigurationManager::configurationChanged, this, &StatusService::handleConnectionChanged_);

	if(SettingsManager::instance().servers().size() > 0)
		settingsSet_ = true;
	else
		settingsSet_ = false;
}

int StatusService::connectionStatus() const
{
	return Client::current().connectionStatus();
}

bool StatusService::wifiEnabled() const
{
	return wifiEnabled_;
}

bool StatusService::settingsSet() const
{
	return settingsSet_;
}

QString StatusService::server() const
{
	if(Client::current().server())
		return Client::current().server()->uuid();
	return "";
}

void StatusService::switchToServer(QString serverUuid)
{
	Client::current().switchToServer(serverUuid);
	if(serverUuid != SettingsManager::instance().lastServer())
	{
		SettingsManager::instance().setLastServer(serverUuid);
		SettingsManager::instance().save();
	}
}

void StatusService::retryConnect()
{
	Client::current().refresh();
}

namespace
{
int serverPropCount(QQmlListProperty<Server>*)
{
	return SettingsManager::instance().servers().size();
}

Server* serverPropAt(QQmlListProperty<Server>*, int index)
{
	return SettingsManager::instance().servers()[index].get();
}

}

QQmlListProperty<Server> StatusService::servers()
{
	return QQmlListProperty<Server>(this, nullptr, &serverPropCount, &serverPropAt);
}

QString StatusService::serverName() const
{
	if(Client::current().server())
		return Client::current().server()->name();
	return "";
}

bool StatusService::atLeastOneWifiConnected()
{
	auto confs = manager_.allConfigurations();
	for(auto conf : confs)
	{
		if(conf.bearerType() == QNetworkConfiguration::BearerWLAN)
		{
			if(conf.state() == QNetworkConfiguration::Active)
				return true;
		}
	}
	return false;
}

void StatusService::updateConnectionStatus(int connectionStatus)
{
	emit connectionStatusChanged(connectionStatus);
}

void StatusService::handleConnectionAdded_(QNetworkConfiguration const& /*config */)
{
	setWifiEnabled(atLeastOneWifiConnected());
}

void StatusService::handleConnectionRemoved_(QNetworkConfiguration const& /*config*/)
{
	setWifiEnabled(atLeastOneWifiConnected());
}

void StatusService::handleConnectionChanged_(QNetworkConfiguration const& /*config */)
{
	setWifiEnabled(atLeastOneWifiConnected());
}

void StatusService::setWifiEnabled(bool wifi)
{
	if(wifi != wifiEnabled_)
	{
		if(!SettingsManager::instance().ignoreWifiStatus())
		{
			wifiEnabled_ = wifi;
			emit wifiEnabledChanged(wifi);
		}
		else if(!wifiEnabled_)
		{
			wifiEnabled_ = true;
			emit wifiEnabledChanged(wifiEnabled_);
		}
	}
}

}
}
}
