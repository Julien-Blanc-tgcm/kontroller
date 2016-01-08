#ifndef EU_TGCM_KONTROLLER_SERVER_H
#define EU_TGCM_KONTROLLER_SERVER_H

#include <QObject>

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

public:
    explicit Server(QObject *parent = 0);
    Q_PROPERTY(QString serverAddress READ serverAddress WRITE setServerAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(int serverHttpPort READ serverHttpPort WRITE setServerHttpPort NOTIFY serverHttpPortChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

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

signals:

void serverAddressChanged(QString serverAddress);

void serverPortChanged(int serverPort);

void serverHttpPortChanged(int serverHttpPort);

void nameChanged(QString name);

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
};

}
}
}

#endif // EU_TGCM_KONTROLLER_SERVER_H
