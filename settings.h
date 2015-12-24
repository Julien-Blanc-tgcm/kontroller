#ifndef EU_TGCM_KONTROLLER_SETTINGS_H
#define EU_TGCM_KONTROLLER_SETTINGS_H

#include <QString>
#include <QObject>
#include "settingsmanager.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(int serverPort READ serverPort NOTIFY serverPortChanged)
    Q_PROPERTY(bool musicFileBrowsing READ musicFileBrowsing WRITE setMusicFileBrowsing NOTIFY musicFileBrowsingChanged)
    Q_PROPERTY(bool videosFileBrowsing READ videosFileBrowsing WRITE setVideosFileBrowsing NOTIFY videosFileBrowsingChanged)
    Q_PROPERTY(bool useHttpInterface READ useHttpInterface WRITE setUseHttpInterface NOTIFY useHttpInterfaceChanged)
    Q_PROPERTY(int dpi READ dpi WRITE setDpi NOTIFY dpiChanged)
    Q_PROPERTY(bool ignoreWifiStatus READ ignoreWifiStatus WRITE setIgnoreWifiStatus NOTIFY ignoreWifiStatusChanged)

public:
    Settings();

    QString serverAddress() const;
    int serverPort() const;
    bool musicFileBrowsing() const;
    bool videosFileBrowsing() const;
    bool useHttpInterface() const;
    int deviceType() const;
    int dpi() const;
    bool ignoreWifiStatus() const;
public slots:
    void setServer(QString address, int port);
    void setMusicFileBrowsing(bool browsing);
    void setVideosFileBrowsing(bool browsing);
    void setUseHttpInterface(bool http);
    void setDeviceType(int type);
    void setDpi(int dpi);
    void setIgnoreWifiStatus(bool value);
signals:
    void serverAddressChanged(QString serverAddress);
    void serverPortChanged(int serverPort);
    void musicFileBrowsingChanged(bool);
    void videosFileBrowsingChanged(bool);
    void useHttpInterfaceChanged(bool);
    void deviceTypeChanged(int);
    void dpiChanged(int);
    void ignoreWifiStatusChanged(bool);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_SETTINGS_H
