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

	QString uuid_;

public:
	explicit Server(QObject *parent = 0);
	Q_PROPERTY(QString serverAddress READ serverAddress WRITE setServerAddress NOTIFY serverAddressChanged)
	Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
	Q_PROPERTY(int serverHttpPort READ serverHttpPort WRITE setServerHttpPort NOTIFY serverHttpPortChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(bool hasZones READ hasZones WRITE setHasZones NOTIFY hasZonesChanged)
	Q_PROPERTY(QStringList zones READ zones WRITE setZones NOTIFY zonesChanged)
	Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)
	QString serverAddress() const;

	int serverPort() const;

	int serverHttpPort() const;

	QString name() const;

	bool hasZones() const;

	QStringList zones() const;

	QString uuid() const;

signals:

	void serverAddressChanged(QString serverAddress);

	void serverPortChanged(int serverPort);

	void serverHttpPortChanged(int serverHttpPort);

	void nameChanged(QString name);

	void hasZonesChanged(bool hasZones);

	void zonesChanged(QStringList zones);


	void uuidChanged(QString uuid);

public slots:
	void setServerAddress(QString serverAddress);
	void setServerPort(int serverPort);
	void setServerHttpPort(int serverHttpPort);
	void setName(QString name);

	void setHasZones(bool hasZones);

	void setZones(QStringList zones);

	void setUuid(QString uuid);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_SERVER_H
