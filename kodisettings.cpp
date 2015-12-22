#include "kodisettings.h"

#include "client.h"
#include "kodisettingsmanager.h"

KodiSettings::KodiSettings()
{

}

void KodiSettings::setServer(QString address, int port)
{
    Client::current().setServerAddress(address);
    Client::current().setServerPort(port);
    Client::current().refresh();
    KodiSettingsManager::instance().setServer(address, port);
    KodiSettingsManager::instance().save();
}

void KodiSettings::setMusicFileBrowsing(bool browsing)
{
    KodiSettingsManager::instance().setMusicFileBrowsing(browsing);
}

void KodiSettings::setVideosFileBrowsing(bool browsing)
{
    KodiSettingsManager::instance().setVideosFileBrowsing(browsing);
}

void KodiSettings::setUseHttpInterface(bool http)
{
    KodiSettingsManager::instance().setUseHttpInterface(http);
}

void KodiSettings::setDeviceType(int type)
{
    KodiSettingsManager::instance().setDeviceType(static_cast<DeviceType>(type));
    emit deviceTypeChanged(type);
}

QString KodiSettings::serverAddress() const
{
    return KodiSettingsManager::instance().serverAddress();
}

int KodiSettings::serverPort() const
{
    return KodiSettingsManager::instance().serverPort();
}

bool KodiSettings::musicFileBrowsing() const
{
    return KodiSettingsManager::instance().musicFileBrowsing();
}

bool KodiSettings::videosFileBrowsing() const
{
    return KodiSettingsManager::instance().videosFileBrowsing();
}

bool KodiSettings::useHttpInterface() const
{
    return KodiSettingsManager::instance().useHttpInterface();
}

int KodiSettings::deviceType() const
{
    return (int) KodiSettingsManager::instance().deviceType();
}

int KodiSettings::dpi() const
{
    return KodiSettingsManager::instance().dpi();
}

void KodiSettings::setDpi(int dpi)
{
    KodiSettingsManager::instance().setDpi(dpi);
    emit dpiChanged(dpi);
}
