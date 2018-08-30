#ifndef EU_TGCM_KONTROLLER_SERVER_H
#define EU_TGCM_KONTROLLER_SERVER_H

#include <QObject>
#include <QStringList>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Server : public QObject
{
    Q_OBJECT
    QString serverAddress_;
    int serverPort_;
    int serverHttpPort_;
    QString name_;
    bool hasZones_;
    QStringList zones_;

public:
    explicit Server(QObject *parent = 0);
    Q_PROPERTY(QString serverAddress READ serverAddress WRITE setServerAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(int serverHttpPort READ serverHttpPort WRITE setServerHttpPort NOTIFY serverHttpPortChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool hasZones READ hasZones WRITE setHasZones NOTIFY hasZonesChanged)
    Q_PROPERTY(QStringList zones READ zones WRITE setZones NOTIFY zonesChanged)

    QString serverAddress() const
    {
        return serverAddress_;
    }

    int serverPort() const
    {
        return serverPort_;
    }

    int serverHttpPort() const
    {
        return serverHttpPort_;
    }

    QString name() const
    {
        return name_;
    }

    bool hasZones() const
    {
        return hasZones_;
    }

    QStringList zones() const
    {
        return zones_;
    }

signals:

void serverAddressChanged(QString serverAddress);

void serverPortChanged(int serverPort);

void serverHttpPortChanged(int serverHttpPort);

void nameChanged(QString name);

void hasZonesChanged(bool hasZones);

void zonesChanged(QStringList zones);


public slots:
void setServerAddress(QString serverAddress)
{
    if (serverAddress_ == serverAddress)
        return;

    serverAddress_ = serverAddress;
    emit serverAddressChanged(serverAddress);
}
void setServerPort(int serverPort)
{
    if (serverPort_ == serverPort)
        return;

    serverPort_ = serverPort;
    emit serverPortChanged(serverPort);
}
void setServerHttpPort(int serverHttpPort)
{
    if (serverHttpPort_ == serverHttpPort)
        return;

    serverHttpPort_ = serverHttpPort;
    emit serverHttpPortChanged(serverHttpPort);
}
void setName(QString name)
{
    if (name_ == name)
        return;

    name_ = name;
    emit nameChanged(name);
}

void setHasZones(bool hasZones)
{
    if(hasZones == hasZones_)
        return;
    hasZones_ = hasZones;
    emit hasZonesChanged(hasZones);
}

void setZones(QStringList zones)
{
    if(zones == zones_)
        return;
    zones_ = zones;
    emit zonesChanged(zones_);
}

};

}
}
}

#endif // EU_TGCM_KONTROLLER_SERVER_H
