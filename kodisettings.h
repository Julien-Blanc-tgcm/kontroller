#ifndef KODISETTINGS_H
#define KODISETTINGS_H

#include <QString>
#include <QObject>
#include "kodisettingsmanager.h"

class KodiSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString serverAddress READ serverAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(int serverPort READ serverPort NOTIFY serverPortChanged)
    Q_PROPERTY(bool musicFileBrowsing READ musicFileBrowsing WRITE setMusicFileBrowsing NOTIFY musicFileBrowsingChanged)
    Q_PROPERTY(bool videosFileBrowsing READ videosFileBrowsing WRITE setVideosFileBrowsing NOTIFY videosFileBrowsingChanged)
    Q_PROPERTY(bool useHttpInterface READ useHttpInterface WRITE setUseHttpInterface NOTIFY useHttpInterfaceChanged)

public:
    KodiSettings();

    QString serverAddress() const;
    int serverPort() const;
    bool musicFileBrowsing() const;
    bool videosFileBrowsing() const;
    bool useHttpInterface() const;
    DeviceType deviceType() const;
public slots:
    void setServer(QString address, int port);
    void setMusicFileBrowsing(bool browsing);
    void setVideosFileBrowsing(bool browsing);
    void setUseHttpInterface(bool http);
signals:
    void serverAddressChanged(QString serverAddress);
    void serverPortChanged(int serverPort);
    void musicFileBrowsingChanged(bool);
    void videosFileBrowsingChanged(bool);
    void useHttpInterfaceChanged(bool);
};

#endif // KODISETTINGS_H
