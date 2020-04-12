#ifndef EU_TGCM_KONTROLLER_SERVER_H
#define EU_TGCM_KONTROLLER_SERVER_H

#include <QObject>
#include <QStringList>

#include "volumeplugin.h"

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

	QString login_;

	QString password_;

	VolumePlugin* volumePlugin_;

public:
	explicit Server(QObject *parent = 0);
	Q_PROPERTY(QString serverAddress READ serverAddress WRITE setServerAddress NOTIFY serverAddressChanged)
	Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)
	Q_PROPERTY(int serverHttpPort READ serverHttpPort WRITE setServerHttpPort NOTIFY serverHttpPortChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(bool hasZones READ hasZones WRITE setHasZones NOTIFY hasZonesChanged)
	Q_PROPERTY(QStringList zones READ zones WRITE setZones NOTIFY zonesChanged)
	Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged)
	Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
	Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
	QString serverAddress() const;

	int serverPort() const;

	int serverHttpPort() const;

	QString name() const;

	bool hasZones() const;

	QStringList zones() const;

	QString uuid() const;

	QString login() const;

	QString password() const;

	VolumePlugin* volumePlugin();

signals:

	void serverAddressChanged(QString serverAddress);

	void serverPortChanged(int serverPort);

	void serverHttpPortChanged(int serverHttpPort);

	void nameChanged(QString name);

	void hasZonesChanged(bool hasZones);

	void zonesChanged(QStringList zones);


	void uuidChanged(QString uuid);

	void loginChanged(QString login);

	void passwordChanged(QString password);

public slots:
	void setServerAddress(QString serverAddress);
	void setServerPort(int serverPort);
	void setServerHttpPort(int serverHttpPort);
	void setName(QString name);

	void setHasZones(bool hasZones);

	void setZones(QStringList zones);

	void setUuid(QString uuid);
	void setLogin(QString login);
	void setPassword(QString password);
	void setVolumePlugin(VolumePlugin* volumePlugin);

	/**
	 * For use by qml, set kodi plugin as the volume one
	 */
	void setKodiVolumePlugin();
	/**
	 * For use by qml, set minidps plugin as the volume one. Takes the ip address
	 */
	void setMinidspVolumePlugin(QString address);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_SERVER_H
