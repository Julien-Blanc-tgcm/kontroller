#ifndef EU_TGCM_KONTROLLER_STATUSSERVICE_H
#define EU_TGCM_KONTROLLER_STATUSSERVICE_H

#include <QObject>
#include <QTimer>
#include <QNetworkConfigurationManager>
#include "settings.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class StatusService : public QObject
{
    Q_OBJECT
    QNetworkConfigurationManager manager_;
    bool wifiEnabled_;
public:
    StatusService(QObject* parent = nullptr);

    Q_PROPERTY(int connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(bool wifiEnabled READ wifiEnabled WRITE setWifiEnabled NOTIFY wifiEnabledChanged)
    Q_PROPERTY(bool settingsSet READ settingsSet WRITE setSettingsSet NOTIFY settingsSetChanged)
    Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::Server> servers READ servers NOTIFY serversChanged)
    Q_PROPERTY(QString server READ server NOTIFY serverChanged)
    int connectionStatus() const;

    bool wifiEnabled() const;
    bool settingsSet() const {
        return settingsSet_;
    }

    QString server() const;

    QQmlListProperty<Server> servers();

public slots:
    void setSettingsSet(bool settingsSet)
    {
        if (settingsSet_ == settingsSet)
            return;

        settingsSet_ = settingsSet;
        emit settingsSetChanged(settingsSet);
    }

    void switchToServer(QString server);
    void retryConnect();

private:
    bool atLeastOneWifiConnected();

    bool settingsSet_;

signals:
    void connectionStatusChanged(int);
    bool wifiEnabledChanged(bool);
    void settingsSetChanged(bool settingsSet);

    void serverChanged();
    void serversChanged();

private slots:
    void updateConnectionStatus(int connectionStatus);
    void setWifiEnabled(bool wifi);
    void handleConnectionAdded_(QNetworkConfiguration const& config);
    void handleConnectionRemoved_(QNetworkConfiguration const& config);
    void handleConnectionChanged_(QNetworkConfiguration const& config);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_STATUSSERVICE_H
