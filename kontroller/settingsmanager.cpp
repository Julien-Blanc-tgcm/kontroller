#include "settingsmanager.h"

#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QStandardPaths>

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
		auto server = new Server{this};
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

		val = settings.value("login");
		if(!val.isNull() && val.canConvert(QVariant::String))
			server->setLogin(val.toString());
		else
			server->setLogin(QString{});
		val = settings.value("password");
		if(!val.isNull() && val.canConvert(QVariant::String))
			server->setPassword(val.toString());
		else
			server->setPassword(QString{});

		servers_.push_back(std::move(server));
	}
	settings.endArray();

	auto val = settings.value("downloadFolder");
	if(!val.isNull() && val.canConvert(QVariant::String))
		setDownloadFolder(val.toString());
	else
		setDownloadFolder(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).front());
	val = settings.value("lastServer");
	if(!val.isNull() && val.canConvert(QVariant::String))
		lastServer_ = val.toString();

	possibleDownloadLocations_.push_back(
	            new DownloadLocation(this));
	possibleDownloadLocations_.back()->setType(DownloadLocation::LocationType::Phone);
	possibleDownloadLocations_.back()->setBaseFolder(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).front());
	QDir dir{"/media/sdcard"};
	if(dir.exists() && dir.isReadable())
	{
		QDirIterator iterator(dir, QDirIterator::IteratorFlag::NoIteratorFlags);
		while(iterator.hasNext())
		{
			iterator.next();
			if(!iterator.fileName().startsWith(".") && iterator.fileInfo().isDir())
			{
				possibleDownloadLocations_.push_back(new DownloadLocation(this));
				possibleDownloadLocations_.back()->setType(DownloadLocation::LocationType::MemoryCard);
				possibleDownloadLocations_.back()->setBaseFolder(iterator.filePath());
			}
		}
	}
}

QString SettingsManager::downloadFolder() const
{
    return downloadFolder_;
}

void SettingsManager::setDownloadFolder(const QString &downloadFolder)
{
	downloadFolder_ = downloadFolder;
}

void SettingsManager::setDownloadLocation(DownloadLocation *downloadLocation)
{
	downloadFolder_ = downloadLocation->baseFolder();
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

QVector<DownloadLocation*> SettingsManager::possibleDownloadLocations() const
{
	return possibleDownloadLocations_;
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

QVector<Server*> SettingsManager::servers()
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
		settings.setValue("login", server->login());
		settings.setValue("password", server->password());
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
			return server;
	}
	return nullptr;
}

Server *SettingsManager::newServer()
{
	servers_.push_back(new Server{this});
	return servers_.back();
}

bool SettingsManager::deleteServer(const QString &uuid)
{
	using namespace std;
	for(auto it = begin(servers_); it != end(servers_); ++it)
	{
		if((*it)->uuid() == uuid)
		{
			(*it)->deleteLater();
			servers_.erase(it);
			return true;
		}
	}
	return false;
}

}
}
}
