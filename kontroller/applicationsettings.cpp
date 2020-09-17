#include "applicationsettings.h"

#include "kodivolumeplugin.h"
#include "minidspvolumeplugin.h"

#include <QSettings>
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
		if(!val.isNull() && val.canConvert(QVariant::String))
		{
			server->setVolumePluginName(val.toString());
			settings.beginGroup(val.toString());
			QVariantMap map;
			for(auto const& k : settings.allKeys())
			{
				map[k] = settings.value(k);
			}
			server->setVolumePluginParameters(map);
			settings.endGroup();
		}
		else
			server->setVolumePluginName(KodiVolumePlugin::static_name());

		val = settings.value("wakeUpPlugin");
		if (!val.isNull() && val.canConvert(QVariant::String))
		{
			server->setWakeUpPluginName(val.toString());
			settings.beginGroup(val.toString());
			QVariantMap map;
			for (auto const& k : settings.allKeys())
			{
				map[k] = settings.value(k);
			}
			server->setWakeUpPluginParameters(map);
			settings.endGroup();
		}

		servers_.push_back(server);
	}
	settings.endArray();

	auto val = settings.value("lastServer");
	if(!val.isNull() && val.canConvert(QVariant::String))
		lastServer_ = val.toString();

	if (servers_.size() == 1) // if only one server, force it to be last server
		lastServer_ = servers_[0]->uuid();

	DownloadLocation phone;
	phone.setType(DownloadLocation::LocationType::Phone);
	phone.setBaseFolder(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).front());
	possibleDownloadLocations_.push_back(phone);
	QDir dir{"/media/sdcard"};
	if(dir.exists() && dir.isReadable())
	{
		QDirIterator iterator(dir, QDirIterator::IteratorFlag::NoIteratorFlags);
		while(iterator.hasNext())
		{
			iterator.next();
			if(!iterator.fileName().startsWith(".") && iterator.fileInfo().isDir())
			{
				DownloadLocation d;
				d.setType(DownloadLocation::LocationType::MemoryCard);
				d.setBaseFolder(iterator.filePath());
				possibleDownloadLocations_.push_back(d);
			}
		}
	}
	val = settings.value("downloadFolder");
	if(!val.isNull() && val.canConvert(QVariant::String))
		setDownloadLocationFolder(val.toString());
	else
		setDownloadLocation(possibleDownloadLocations_.front());

}

DownloadLocation ApplicationSettings::downloadLocation() const
{
	return downloadLocation_;
}

void ApplicationSettings::setDownloadLocationFolder(const QString& folder)
{
	for(auto& l : possibleDownloadLocations_)
	{
		if(l.baseFolder() == folder)
		{
			setDownloadLocation(l);
			return;
		}
	}
	if(!possibleDownloadLocations_.empty())
		setDownloadLocation(possibleDownloadLocations_[0]);
}

void ApplicationSettings::setDownloadLocation(DownloadLocation const& downloadLocation)
{
	if(downloadLocation_ == downloadLocation)
		return;
	downloadLocation_ = downloadLocation;
	emit downloadLocationChanged();
	emit downloadLocationIndexChanged();
}

void ApplicationSettings::setDownloadLocationIndex(int index)
{
	if(index < possibleDownloadLocations_.size())
	{
		setDownloadLocation(possibleDownloadLocations_[index]);
	}
}

int ApplicationSettings::downloadLocationIndex() const
{
	for(int i = 0; i <possibleDownloadLocations_.size(); ++i)
	{
		if(possibleDownloadLocations_[i] == downloadLocation_)
		{
			return i;
		}
	}
	return -1;
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
	if(ignoreWifiStatus == ignoreWifiStatus_)
		return;
	ignoreWifiStatus_ = ignoreWifiStatus;
	emit ignoreWifiStatusChanged(ignoreWifiStatus_);
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
		settings.setValue("volumePlugin", server->volumePluginName());
		settings.beginGroup(server->volumePluginName());
		for(auto it = server->volumePluginParameters().begin(); it != server->volumePluginParameters().end(); ++it)
		{
			settings.setValue(it.key(), it.value());
		}
		settings.endGroup();
		if(server->wakeUpPluginName() != "")
		{
			settings.setValue("wakeUpPlugin", server->wakeUpPluginName());
			settings.beginGroup(server->wakeUpPluginName());
			for (auto it = server->wakeUpPluginParameters().begin(); it != server->wakeUpPluginParameters().end(); ++it)
			{
				settings.setValue(it.key(), it.value());
			}
			settings.endGroup();
		}
		i += 1;
	}
	settings.endArray();
#ifndef SAILFISH_TARGET
	settings.setValue("dpi", dpi_);
#endif
	settings.setValue("downloadFolder", downloadLocation().baseFolder());
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
	ret->setVolumePluginName(KodiVolumePlugin::static_name());
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
	return QQmlListProperty<Server>(
	    this, this, &ApplicationSettings::getServerCount_, &ApplicationSettings::getServerAt_);
}

QVariantList ApplicationSettings::possibleDownloadLocations()
{
	QVariantList list;
	for(auto const& d : possibleDownloadLocations_)
		list.push_back(QVariant::fromValue(d));
	return list;
}

}
}
}
