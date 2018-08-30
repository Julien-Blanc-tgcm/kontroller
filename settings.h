#ifndef EU_TGCM_KONTROLLER_SETTINGS_H
#define EU_TGCM_KONTROLLER_SETTINGS_H

#include <QString>
#include <QObject>
#include "settingsmanager.h"
#include <QQmlListProperty>
#include <QTimer>

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
    QTimer* timer_;

public:
    Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::Server> servers READ servers NOTIFY serversChanged)
    Q_PROPERTY(int currentServerIdx READ currentServerIdx WRITE setCurrentServerIdx NOTIFY currentServerIdxChanged)

    Q_PROPERTY(bool ignoreWifiStatus READ ignoreWifiStatus WRITE setIgnoreWifiStatus NOTIFY ignoreWifiStatusChanged)
#ifndef SAILFISH_TARGET
    Q_PROPERTY(int dpi READ dpi WRITE setDpi NOTIFY dpiChanged)
#endif

public:
    Settings();

    QQmlListProperty<Server> servers();
    int currentServerIdx() const;

    bool ignoreWifiStatus() const
    {
        return ignoreWifiStatus_;
    }

#ifndef SAILFISH_TARGET
    int dpi() const;
#endif

public slots:
    void setCurrentServerIdx(int idx);
    void save();
    void newServer(QString serverName);
    void removeCurrentServer();

    void setIgnoreWifiStatus(bool ignoreWifiStatus)
    {
        if (ignoreWifiStatus_ == ignoreWifiStatus)
            return;

        ignoreWifiStatus_ = ignoreWifiStatus;
        emit ignoreWifiStatusChanged(ignoreWifiStatus);
    }
#ifndef SAILFISH_TARGET
    void setDpi(int dpi);
#endif

    void pollForZones();
    void endPolling();

signals:
    void serversChanged();
    void currentServerIdxChanged();
    void ignoreWifiStatusChanged(bool ignoreWifiStatus);
#ifndef SAILFISH_TARGET
    void dpiChanged(int dpi);
#endif

private slots:
    void pollCurrentZone();
    void pollCurrentZoneReply();
};

}
}
}

#endif // EU_TGCM_KONTROLLER_SETTINGS_H
