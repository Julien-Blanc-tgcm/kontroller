#include "statusservice.h"

#include "applicationsettings.h"
#include "client.h"

#include <QNetworkConfigurationManager>


namespace eu
{
namespace tgcm
{
namespace kontroller
{

StatusService::StatusService(QObject* parent) :
    QObject(parent)
{
	connect(&manager_, &QNetworkConfigurationManager::configurationAdded, this, &StatusService::handleConnectionAdded_);
	connect(&manager_, &QNetworkConfigurationManager::configurationRemoved, this, &StatusService::handleConnectionRemoved_);
	connect(&manager_, &QNetworkConfigurationManager::configurationChanged, this, &StatusService::handleConnectionChanged_);
}

bool StatusService::wifiEnabled() const
{
	return wifiEnabled_;
}

ApplicationSettings* StatusService::settings() const
{
	return settings_;
}

void StatusService::setSettings(ApplicationSettings* settings)
{
	if (settings_ == settings)
		return;

	settings_ = settings;
	setWifiEnabled(atLeastOneWifiConnected());
	emit settingsChanged(settings_);
}

bool StatusService::atLeastOneWifiConnected()
{
	auto confs = manager_.allConfigurations();
	for(auto const& conf : confs)
	{
		if(conf.bearerType() == QNetworkConfiguration::BearerWLAN)
		{
			if(conf.state() == QNetworkConfiguration::Active)
				return true;
		}
	}
	return false;
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
		if(!settings_->ignoreWifiStatus())
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
