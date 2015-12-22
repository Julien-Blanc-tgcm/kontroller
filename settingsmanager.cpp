#include "settingsmanager.h"

#include <QSettings>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

SettingsManager::SettingsManager()
{
    QSettings settings("tgcm.eu", "kontroller");
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
    val = settings.value("deviceType");
    int valDt = -1;
    if(!val.isNull() && val.canConvert(QVariant::Int))
        valDt = val.toInt();
    if(valDt > (int) DeviceType::Undefined && valDt <= (int) DeviceType::TV)
        deviceType_ = static_cast<DeviceType>(valDt);
    else
        deviceType_ = DeviceType::Undefined;

    val = settings.value("dpi");
    if(!val.isNull() && val.canConvert(QVariant::Int))
        dpi_ = val.toInt();
    else
        dpi_ = 0;
}

bool SettingsManager::useHttpInterface() const
{
    return m_useHttpInterface;
}

void SettingsManager::setUseHttpInterface(bool useHttpInterface)
{
    m_useHttpInterface = useHttpInterface;
}

DeviceType SettingsManager::deviceType() const
{
    return deviceType_;
}

void SettingsManager::setDeviceType(DeviceType type)
{
    deviceType_ = type;
}

int SettingsManager::dpi() const
{
    return dpi_;
}

void SettingsManager::setDpi(int dpi)
{
    dpi_ = dpi;
}

SettingsManager& SettingsManager::instance()
{
    static SettingsManager manager;
    return manager;
}

QString SettingsManager::serverAddress() const
{
    return m_serverAddress;
}

int SettingsManager::serverPort() const
{
    return m_serverPort;
}

bool SettingsManager::musicFileBrowsing() const
{
    return m_musicFileBrowsing;
}

bool SettingsManager::videosFileBrowsing() const
{
    return m_videosFileBrowsing;
}

void SettingsManager::setServer(QString address, int port)
{
    m_serverAddress = address;
    m_serverPort = port;
}

void SettingsManager::setMusicFileBrowsing(bool browsing)
{
    m_musicFileBrowsing = browsing;
}

void SettingsManager::setVideosFileBrowsing(bool browsing)
{
    m_videosFileBrowsing = browsing;
}

void SettingsManager::save()
{
    QSettings settings("tgcm.eu", "kontroller");
    settings.setValue("server", m_serverAddress);
    settings.setValue("port", m_serverPort);
    settings.setValue("musicFileBrowsing", m_musicFileBrowsing);
    settings.setValue("videosFileBrowsing", m_videosFileBrowsing);
    settings.setValue("useHttpInterface", m_useHttpInterface);
    settings.setValue("dpi", dpi_);
    settings.setValue("deviceType", (int)deviceType_);
    settings.sync();
}


}
}
}
