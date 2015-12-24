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
        serverAddress_ = val.toString();
    else
        serverAddress_ = "";
    val = settings.value("port");
    if(val.canConvert(QVariant::Int))
        serverPort_ = val.toInt();
    else
        serverPort_ = 9090;
    val = settings.value("musicFileBrowsing");
    if(val.canConvert(QVariant::Bool))
        musicFileBrowsing_ = val.toBool();
    else
        musicFileBrowsing_ = false;
    val = settings.value("videosFileBrowsing");
    if(val.canConvert(QVariant::Bool))
        videosFileBrowsing_ = val.toBool();
    else
        videosFileBrowsing_ = false;
    val = settings.value("useHttpInterface");
    if(val.canConvert(QVariant::Bool))
        useHttpInterface_ = val.toBool();
    else
        useHttpInterface_ = serverPort_ == 8080;
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

    val = settings.value("ignoreWifiStatus");
    if(!val.isNull() && val.canConvert(QVariant::Bool))
        ignoreWifiStatus_ = val.toBool();
    else
        ignoreWifiStatus_ = false;
}

bool SettingsManager::ignoreWifiStatus() const
{
    return ignoreWifiStatus_;
}

void SettingsManager::setIgnoreWifiStatus(bool ignoreWifiStatus)
{
    ignoreWifiStatus_ = ignoreWifiStatus;
}

bool SettingsManager::useHttpInterface() const
{
    return useHttpInterface_;
}

void SettingsManager::setUseHttpInterface(bool useHttpInterface)
{
    useHttpInterface_ = useHttpInterface;
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
    return serverAddress_;
}

int SettingsManager::serverPort() const
{
    return serverPort_;
}

bool SettingsManager::musicFileBrowsing() const
{
    return musicFileBrowsing_;
}

bool SettingsManager::videosFileBrowsing() const
{
    return videosFileBrowsing_;
}

void SettingsManager::setServer(QString address, int port)
{
    serverAddress_ = address;
    serverPort_ = port;
}

void SettingsManager::setMusicFileBrowsing(bool browsing)
{
    musicFileBrowsing_ = browsing;
}

void SettingsManager::setVideosFileBrowsing(bool browsing)
{
    videosFileBrowsing_ = browsing;
}

void SettingsManager::save()
{
    QSettings settings("tgcm.eu", "kontroller");
    settings.setValue("server", serverAddress_);
    settings.setValue("port", serverPort_);
    settings.setValue("musicFileBrowsing", musicFileBrowsing_);
    settings.setValue("videosFileBrowsing", videosFileBrowsing_);
    settings.setValue("useHttpInterface", useHttpInterface_);
    settings.setValue("dpi", dpi_);
    settings.setValue("deviceType", (int)deviceType_);
    settings.setValue("ignoreWifiStatus", ignoreWifiStatus_);
    settings.sync();
}


}
}
}
