#include "applicationsettings.h"

#include "kodivolumeplugin.h"
#include "minidspvolumeplugin.h"

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

ApplicationSettings::ApplicationSettings(QObject* parent) :
    QObject{parent}
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
		val = settings.value("volumePlugin");
		if(val.isNull() || !val.canConvert(QVariant::String) || val.toString() == KodiVolumePlugin::static_name())
			server->setKodiVolumePlugin();
		else if(val.toString() == MinidspVolumePlugin::static_name())
		{
			settings.beginGroup(MinidspVolumePlugin::static_name());
			val = settings.value("address");
			if(val.canConvert(QVariant::String))
				server->setMinidspVolumePlugin(val.toString());
			settings.endGroup();
		}
		if(server->volumePlugin() == nullptr) // failsafe, in case volume plugin init failed for some reason
			server->setVolumePlugin(new KodiVolumePlugin(server));
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

QString ApplicationSettings::downloadFolder() const
{
    return downloadFolder_;
}

void ApplicationSettings::setDownloadFolder(const QString &downloadFolder)
{
	downloadFolder_ = downloadFolder;
}

void ApplicationSettings::setDownloadLocation(DownloadLocation *downloadLocation)
{
	if(downloadLocation != nullptr)
		downloadFolder_ = downloadLocation->baseFolder();
}

void ApplicationSettings::setLastServer(QString lastServerUuid)
{
	lastServer_ = lastServerUuid;
	save();
}

QString ApplicationSettings::lastServer() const
{
	return lastServer_;
}

int ApplicationSettings::lastServerIndex() const
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

bool ApplicationSettings::ignoreWifiStatus() const
{
    return ignoreWifiStatus_;
}

void ApplicationSettings::setIgnoreWifiStatus(bool ignoreWifiStatus)
{
    ignoreWifiStatus_ = ignoreWifiStatus;
}

/*DeviceType ApplicationSettings::deviceType() const
{
    return deviceType_;
}

void ApplicationSettings::setDeviceType(DeviceType type)
{
    deviceType_ = type;
}*/
#ifndef SAILFISH_TARGET
int ApplicationSettings::dpi() const
{
    return dpi_;
}

void ApplicationSettings::setDpi(int dpi)
{
    dpi_ = dpi;
}
#endif

void ApplicationSettings::save()
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
		settings.setValue("uuid", server->uuid());
		settings.setValue("port", server->serverPort());
		settings.setValue("serverHttpPort", server->serverHttpPort());
		settings.setValue("hasZones", server->hasZones());
		settings.setValue("zones", server->zones());
		settings.setValue("login", server->login());
		settings.setValue("password", server->password());
		settings.setValue("volumePlugin", server->volumePlugin()->name());
		if(server->volumePlugin()->name() == MinidspVolumePlugin::static_name())
		{
			settings.beginGroup(MinidspVolumePlugin::static_name());
			settings.setValue("address", dynamic_cast<MinidspVolumePlugin*>(server->volumePlugin())->ipAddress());
			settings.endGroup();
		}
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

Server* ApplicationSettings::server(const QString &uuid)
{
	for(auto& server: servers_)
	{
		if(server->uuid() == uuid)
			return server;
	}
	return nullptr;
}

Server *ApplicationSettings::newServer()
{
	servers_.push_back(new Server{this});
	auto ret = servers_.back();
	ret->setVolumePlugin(new KodiVolumePlugin(ret));
	emit serversChanged();
	return ret;
}

bool ApplicationSettings::deleteServer(const QString &uuid)
{
	using namespace std;
	for(auto it = begin(servers_); it != end(servers_); ++it)
	{
		if((*it)->uuid() == uuid)
		{
			(*it)->deleteLater();
			servers_.erase(it);
			emit serversChanged();
			save();
			return true;
		}
	}
	return false;
}

int ApplicationSettings::getServerCount_(QQmlListProperty<Server>* list)
{
	return static_cast<ApplicationSettings*>(list->data)->servers_.size();
}

Server* ApplicationSettings::getServerAt_(QQmlListProperty<Server>* list, int index)
{
	return static_cast<ApplicationSettings*>(list->data)->servers_.at(index);
}


QQmlListProperty<Server> ApplicationSettings::servers()
{
	return QQmlListProperty<Server>(this, this, &ApplicationSettings::getServerCount_,
	                                &ApplicationSettings::getServerAt_);
}

int ApplicationSettings::getDownloadLocationCount_(QQmlListProperty<DownloadLocation>* list)
{
	return static_cast<ApplicationSettings*>(list->data)->possibleDownloadLocations_.size();
}

DownloadLocation* ApplicationSettings::getDownloadLocationAt_(QQmlListProperty<DownloadLocation>* list, int index)
{
	return static_cast<ApplicationSettings*>(list->data)->possibleDownloadLocations_.at(index);
}

QQmlListProperty<DownloadLocation> ApplicationSettings::possibleDownloadLocations()
{
	return QQmlListProperty<DownloadLocation>(this, this, &ApplicationSettings::getDownloadLocationCount_,
	                                          &ApplicationSettings::getDownloadLocationAt_);
}

}
}
}
