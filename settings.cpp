#include "settings.h"

#include "client.h"
#include "settingsmanager.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

Settings::Settings()
{

}

void Settings::setServer(QString address, int port)
{
    Client::current().setServerAddress(address);
    Client::current().setServerPort(port);
    Client::current().refresh();
    SettingsManager::instance().setServer(address, port);
    SettingsManager::instance().save();
}

void Settings::setMusicFileBrowsing(bool browsing)
{
    SettingsManager::instance().setMusicFileBrowsing(browsing);
}

void Settings::setVideosFileBrowsing(bool browsing)
{
    SettingsManager::instance().setVideosFileBrowsing(browsing);
}

void Settings::setUseHttpInterface(bool http)
{
    SettingsManager::instance().setUseHttpInterface(http);
}

void Settings::setDeviceType(int type)
{
    SettingsManager::instance().setDeviceType(static_cast<DeviceType>(type));
    emit deviceTypeChanged(type);
}

QString Settings::serverAddress() const
{
    return SettingsManager::instance().serverAddress();
}

int Settings::serverPort() const
{
    return SettingsManager::instance().serverPort();
}

bool Settings::musicFileBrowsing() const
{
    return SettingsManager::instance().musicFileBrowsing();
}

bool Settings::videosFileBrowsing() const
{
    return SettingsManager::instance().videosFileBrowsing();
}

bool Settings::useHttpInterface() const
{
    return SettingsManager::instance().useHttpInterface();
}

int Settings::deviceType() const
{
    return (int) SettingsManager::instance().deviceType();
}

int Settings::dpi() const
{
    return SettingsManager::instance().dpi();
}

void Settings::setDpi(int dpi)
{
    SettingsManager::instance().setDpi(dpi);
    emit dpiChanged(dpi);
}

}
}
}
