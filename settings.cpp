#include "settings.h"

#include "client.h"
#include "settingsmanager.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

Settings::Settings() :
    currentServerIdx_(0),
    timer_{new QTimer(this)}
{
	if(SettingsManager::instance().servers().size() == 0)
	{
		SettingsManager::instance().newServer()->setName(tr("Default"));
	}
	else
	{
		setCurrentServerIdx(SettingsManager::instance().lastServerIndex());
	}
	timer_->setInterval(2000);
	timer_->setSingleShot(false);
	connect(timer_, SIGNAL(timeout()), this, SLOT(pollCurrentZone()));
}

void Settings::setCurrentServerIdx(int idx)
{
	if(idx != currentServerIdx_)
	{
		currentServerIdx_ = idx;
		emit currentServerIdxChanged();
	}
}

void Settings::save()
{
	SettingsManager::instance().save();
	Client::current().refresh();
}

#ifndef SAILFISH_TARGET
int Settings::dpi() const
{
	return SettingsManager::instance().dpi();
}

void Settings::setDpi(int dpi)
{
	SettingsManager::instance().setDpi(dpi);
	emit dpiChanged(dpi);
}
#endif

namespace
{
int getServerCount(QQmlListProperty<Server>*)
{
	return SettingsManager::instance().servers().size();
}

Server* getServerAt(QQmlListProperty<Server>*, int index )
{
	return SettingsManager::instance().servers()[index];
}
}

QQmlListProperty<Server> Settings::servers()
{
	return QQmlListProperty<Server>(this, nullptr, getServerCount, getServerAt);
}

int Settings::currentServerIdx() const
{
	return currentServerIdx_;
}

bool Settings::ignoreWifiStatus() const
{
	return SettingsManager::instance().ignoreWifiStatus();
}

QString Settings::downloadFolder() const
{
	return SettingsManager::instance().downloadFolder();
}

namespace
{
int getDownloadLocationCount(QQmlListProperty<DownloadLocation>*)
{
	return SettingsManager::instance().possibleDownloadLocations().size();
}

DownloadLocation* getDownloadLocationAt(QQmlListProperty<DownloadLocation>*, int index )
{
	return SettingsManager::instance().possibleDownloadLocations()[index];
}
}

QQmlListProperty<DownloadLocation> Settings::possibleDownloadFolders()
{
	return QQmlListProperty<DownloadLocation>(this, nullptr, getDownloadLocationCount, getDownloadLocationAt);
}

Server *Settings::server(QString uuid)
{
	return SettingsManager::instance().server(uuid);
}

QString Settings::newServer()
{
	auto ret = SettingsManager::instance().newServer();
	emit serversChanged();
	return ret->uuid();
	//->setName(serverName);
	//setCurrentServerIdx(SettingsManager::instance().servers().size() - 1);
}

void Settings::removeCurrentServer()
{
	if(SettingsManager::instance().servers().size() > 1)
	{
		auto it = SettingsManager::instance().servers().begin();
		std::advance(it, currentServerIdx_);
		if(it != SettingsManager::instance().servers().end())
			SettingsManager::instance().servers().erase(it);
		setCurrentServerIdx(0);
		emit serversChanged();
	}
}

void Settings::setIgnoreWifiStatus(bool ignoreWifiStatus)
{
	bool old = SettingsManager::instance().ignoreWifiStatus();
	if (old == ignoreWifiStatus)
		return;
	SettingsManager::instance().setIgnoreWifiStatus(ignoreWifiStatus);

	emit ignoreWifiStatusChanged(ignoreWifiStatus);
}

void Settings::pollForZones()
{
	Server* server = getServerAt(nullptr, currentServerIdx());
	if(server)
	{
		Client::current().switchToServer(server->name());
		timer_->start();
	}
}

void Settings::pollCurrentZone()
{
	QJsonObject params;
	params.insert("setting", QString{"audiooutput.audiodevice"});
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Settings.GetSettingValue", params);
	auto reply = Client::current().send(message);
	connect(reply, SIGNAL(finished()), this, SLOT(pollCurrentZoneReply()));
}

void Settings::endPolling()
{
	timer_->stop();
}

void Settings::setDownloadFolder(QString downloadFolder)
{
	QString old = SettingsManager::instance().downloadFolder();
	if(old == downloadFolder)
		return;

	SettingsManager::instance().setDownloadFolder(downloadFolder);
	emit downloadFolderChanged(downloadFolder);
}

void Settings::updateDownloadFolder(int idx)
{
	if(idx < 0)
		return;
	if(idx >= SettingsManager::instance().possibleDownloadLocations().size())
		return;
	SettingsManager::instance().setDownloadLocation(
	            SettingsManager::instance().possibleDownloadLocations()[idx]);
}

void Settings::pollCurrentZoneReply()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	auto resp = reply->response().toObject();
	if(resp["result"].isObject())
	{
		auto result = resp["result"].toObject();
		auto zones = getServerAt(nullptr, currentServerIdx())->zones();
		QString zone = result["value"].toString();
		if(!zones.contains(zone))
		{
			zones.append(zone);
			getServerAt(nullptr, currentServerIdx_)->setZones(zones);
		}
	}
}

}
}
}
