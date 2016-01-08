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
    currentServerIdx_(0)
{
    if(SettingsManager::instance().servers().size() == 0)
        newServer(tr("Default"));
    else
    {
        setCurrentServerIdx(0);
    }
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

//void Settings::setDeviceType(int type)
//{
//    SettingsManager::instance().setDeviceType(static_cast<DeviceType>(type));
//    emit deviceTypeChanged(type);
//}

//int Settings::deviceType() const
//{
//    return (int) SettingsManager::instance().deviceType();
//}

//int Settings::dpi() const
//{
//    return SettingsManager::instance().dpi();
//}

//void Settings::setDpi(int dpi)
//{
//    SettingsManager::instance().setDpi(dpi);
//    emit dpiChanged(dpi);
//}

namespace
{
int getServerCount(QQmlListProperty<Server>*)
{
    return SettingsManager::instance().servers().size();
}

Server* getServerAt(QQmlListProperty<Server>*, int index )
{
    return SettingsManager::instance().servers()[index].get();
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

void Settings::newServer(QString serverName)
{
    SettingsManager::instance().servers().emplace_back(new Server);
    emit serversChanged();
    SettingsManager::instance().servers().back()->setName(serverName);
    setCurrentServerIdx(SettingsManager::instance().servers().size() - 1);
}

}
}
}
