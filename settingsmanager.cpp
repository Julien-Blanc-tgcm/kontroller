#include "settingsmanager.h"

#include <QSettings>
#include <QDebug>

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
        std::unique_ptr<Server> server{new Server};
        QVariant val;
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
        servers_.push_back(std::move(server));
    }
    settings.endArray();
/*    val = settings.value("musicFileBrowsing");
    if(val.canConvert(QVariant::Bool))
        musicFileBrowsing_ = val.toBool();
    else
        musicFileBrowsing_ = false;
    val = settings.value("videosFileBrowsing");
    if(val.canConvert(QVariant::Bool))
        videosFileBrowsing_ = val.toBool();
    else
        videosFileBrowsing_ = false; */
/*    val = settings.value("deviceType");
    int valDt = -1;
    if(!val.isNull() && val.canConvert(QVariant::Int))
        valDt = val.toInt();
    if(valDt > (int) DeviceType::Undefined && valDt <= (int) DeviceType::TV)
        deviceType_ = static_cast<DeviceType>(valDt);
    else
        deviceType_ = DeviceType::Undefined; */

/*    val = settings.value("dpi");
    if(!val.isNull() && val.canConvert(QVariant::Int))
        dpi_ = val.toInt();
    else
        dpi_ = 0; */

/*    val = settings.value("ignoreWifiStatus");
    if(!val.isNull() && val.canConvert(QVariant::Bool))
        ignoreWifiStatus_ = val.toBool();
    else
        ignoreWifiStatus_ = false; */

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

/*int SettingsManager::dpi() const
{
    return dpi_;
}

void SettingsManager::setDpi(int dpi)
{
    dpi_ = dpi;
}*/

SettingsManager& SettingsManager::instance()
{
    static SettingsManager manager;
    return manager;
}

std::vector<std::unique_ptr<Server> > &SettingsManager::servers()
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
        i += 1;
    }
    settings.endArray();
//    settings.setValue("dpi", dpi_);
//    settings.setValue("deviceType", (int)deviceType_);
//    settings.setValue("ignoreWifiStatus", ignoreWifiStatus_);
    settings.sync();
}

Server* SettingsManager::server(const QString &name)
{
    for(auto& server: servers_)
    {
        if(server->name() == name)
            return server.get();
    }
    return nullptr;
}


}
}
}
