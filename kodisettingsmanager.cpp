#include "kodisettingsmanager.h"

#include <QSettings>

KodiSettingsManager::KodiSettingsManager()
{
    QSettings settings("tgcm.eu", "kodiremote");
    QVariant val;
    val = settings.value("server");
    if(val.canConvert(QVariant::String))
        m_serverAddress = val.toString();
    else
        m_serverAddress = "";
    val = settings.value("port");
    if(val.canConvert(QVariant::Int))
        m_serverPort = val.toInt();
    else
        m_serverPort = 9090;
    val = settings.value("musicFileBrowsing");
    if(val.canConvert(QVariant::Bool))
        m_musicFileBrowsing = val.toBool();
    else
        m_musicFileBrowsing = false;
    val = settings.value("videosFileBrowsing");
    if(val.canConvert(QVariant::Bool))
        m_videosFileBrowsing = val.toBool();
    else
        m_videosFileBrowsing = false;
    val = settings.value("useHttpInterface");
    if(val.canConvert(QVariant::Bool))
        m_useHttpInterface = val.toBool();
    else
        m_useHttpInterface = m_serverPort == 8080;
}

bool KodiSettingsManager::useHttpInterface() const
{
    return m_useHttpInterface;
}

void KodiSettingsManager::setUseHttpInterface(bool useHttpInterface)
{
    m_useHttpInterface = useHttpInterface;
}

KodiSettingsManager& KodiSettingsManager::instance()
{
    static KodiSettingsManager manager;
    return manager;
}

QString KodiSettingsManager::serverAddress() const
{
    return m_serverAddress;
}

int KodiSettingsManager::serverPort() const
{
    return m_serverPort;
}

bool KodiSettingsManager::musicFileBrowsing() const
{
    return m_musicFileBrowsing;
}

bool KodiSettingsManager::videosFileBrowsing() const
{
    return m_videosFileBrowsing;
}

void KodiSettingsManager::setServer(QString address, int port)
{
    m_serverAddress = address;
    m_serverPort = port;
}

void KodiSettingsManager::setMusicFileBrowsing(bool browsing)
{
    m_musicFileBrowsing = browsing;
}

void KodiSettingsManager::setVideosFileBrowsing(bool browsing)
{
    m_videosFileBrowsing = browsing;
}

void KodiSettingsManager::save()
{
    QSettings settings("tgcm.eu", "kodiremote");
    settings.setValue("server", m_serverAddress);
    settings.setValue("port", m_serverPort);
    settings.setValue("musicFileBrowsing", m_musicFileBrowsing);
    settings.setValue("videosFileBrowsing", m_videosFileBrowsing);
    settings.setValue("useHttpInterface", m_useHttpInterface);
    settings.sync();
}
