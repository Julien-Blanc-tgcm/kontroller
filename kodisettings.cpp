#include "kodisettings.h"

#include "kodiclient.h"
#include "kodisettingsmanager.h"

KodiSettings::KodiSettings()
{

}

void KodiSettings::setServer(QString address, int port)
{
    KodiClient::current().setServerAddress(address);
    KodiClient::current().setServerPort(port);
    KodiClient::current().refresh();
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
