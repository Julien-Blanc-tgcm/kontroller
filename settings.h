#ifndef EU_TGCM_KONTROLLER_SETTINGS_H
#define EU_TGCM_KONTROLLER_SETTINGS_H

#include <QString>
#include <QObject>
#include "settingsmanager.h"
#include <QQmlListProperty>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Settings : public QObject
{
    Q_OBJECT

    bool ignoreWifiStatus_;
    int currentServerIdx_;


public:
    Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::Server> servers READ servers NOTIFY serversChanged)
    Q_PROPERTY(int currentServerIdx READ currentServerIdx WRITE setCurrentServerIdx NOTIFY currentServerIdxChanged)

    Q_PROPERTY(bool ignoreWifiStatus READ ignoreWifiStatus WRITE setIgnoreWifiStatus NOTIFY ignoreWifiStatusChanged)

public:
    Settings();

    QQmlListProperty<Server> servers();
    int currentServerIdx() const;

    bool ignoreWifiStatus() const
    {
        return ignoreWifiStatus_;
    }

public slots:
    void setCurrentServerIdx(int idx);
    void save();
    void newServer(QString serverName);

    void setIgnoreWifiStatus(bool ignoreWifiStatus)
    {
        if (ignoreWifiStatus_ == ignoreWifiStatus)
            return;

        ignoreWifiStatus_ = ignoreWifiStatus;
        emit ignoreWifiStatusChanged(ignoreWifiStatus);
    }

signals:
    void serversChanged();
    void currentServerIdxChanged();
    void ignoreWifiStatusChanged(bool ignoreWifiStatus);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_SETTINGS_H
