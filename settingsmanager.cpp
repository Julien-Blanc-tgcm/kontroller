#include "settingsmanager.h"

#include <QSettings>
#include <QDebug>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

SettingsManager::SettingsManager()
{
	QSettings settings("tgcm.eu", "kontroller");
	auto nbServers = settings.beginReadArray("servers");
	for(auto i = 0; i < nbServers; ++i)
	{
		settings.setArrayIndex(i);
		std::unique_ptr<Server> server{new Server};
		QVariant val;
		val = settings.value("uuid");
		if(val.canConvert(QVariant::String))
			server->setUuid(val.toString());
		val = settings.value("name");
		if(val.canConvert(QVariant::String))
			server->setName(val.toString());
		val = settings.value("server");
		if(val.canConvert(QVariant::String))
			server->setServerAddress(val.toString());
		else
			server->setServerAddress("");
		val = settings.value("port");
		if(val.canConvert(QVariant::Int))
			server->setServerPort(val.toInt());
		else
			server->setServerPort(9090);
		/*        val = settings.value("useHttpInterface");
		if(val.canConvert(QVariant::Bool))
			useHttpInterface_ = val.toBool();
		else
			useHttpInterface_ = serverPort_ == 8080; */

		val = settings.value("serverHttpPort");
		if(!val.isNull() && val.canConvert(QVariant::Int))
			server->setServerHttpPort(val.toInt());
		else
			server->setServerHttpPort(8080);

		servers_.push_back(std::move(server));
	}
	settings.endArray();

	auto val = settings.value("downloadFolder");
	if(!val.isNull() && val.canConvert(QVariant::String))
		setDownloadFolder(val.toString());
	else
		setDownloadFolder("/home/nemo/Music");
	val = settings.value("lastServer");
	if(!val.isNull() && val.canConvert(QVariant::String))
		lastServer_ = val.toString();
}

QString SettingsManager::downloadFolder() const
{
    return downloadFolder_;
}

void SettingsManager::setDownloadFolder(const QString &downloadFolder)
{
	downloadFolder_ = downloadFolder;
}

void SettingsManager::setLastServer(QString lastServerUuid)
{
	lastServer_ = lastServerUuid;
	save();
}

QString SettingsManager::lastServer() const
{
	return lastServer_;
}

int SettingsManager::lastServerIndex() const
{
	int i = 0;
	for(auto& server : servers_)
	{
		if(server->uuid() == lastServer_)
			return i;
		++i;
	}
	return 0;
}

bool SettingsManager::ignoreWifiStatus() const
{
    return ignoreWifiStatus_;
}

void SettingsManager::setIgnoreWifiStatus(bool ignoreWifiStatus)
{
    ignoreWifiStatus_ = ignoreWifiStatus;
}

/*DeviceType SettingsManager::deviceType() const
{
    return deviceType_;
}

void SettingsManager::setDeviceType(DeviceType type)
{
    deviceType_ = type;
}*/
#ifndef SAILFISH_TARGET
int SettingsManager::dpi() const
{
    return dpi_;
}

void SettingsManager::setDpi(int dpi)
{
    dpi_ = dpi;
}
#endif

SettingsManager& SettingsManager::instance()
{
    static SettingsManager manager;
    return manager;
}

std::vector<std::unique_ptr<Server> > &SettingsManager::servers()
{
    return servers_;
}

void SettingsManager::save()
{
	QSettings settings("tgcm.eu", "kontroller");
	settings.clear();
	settings.beginWriteArray("servers", servers_.size());
	int i = 0;
	for(auto& server : servers_)
	{
		settings.setArrayIndex(i);
		settings.setValue("name", server->name());
		settings.setValue("server", server->serverAddress());
		settings.setValue("port", server->serverPort());
		settings.setValue("serverHttpPort", server->serverHttpPort());
		settings.setValue("hasZones", server->hasZones());
		settings.setValue("zones", server->zones());
		i += 1;
	}
	settings.endArray();
#ifndef SAILFISH_TARGET
    settings.setValue("dpi", dpi_);
#endif
	settings.setValue("downloadFolder", downloadFolder());
	settings.setValue("lastServer", lastServer());
//    settings.setValue("deviceType", (int)deviceType_);
//    settings.setValue("ignoreWifiStatus", ignoreWifiStatus_);
	settings.sync();
}

Server* SettingsManager::server(const QString &uuid)
{
	for(auto& server: servers_)
	{
		if(server->uuid() == uuid)
			return server.get();
	}
	return nullptr;
}


}
}
}
