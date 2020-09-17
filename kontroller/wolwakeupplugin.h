#ifndef WOLWAKEUPPLUGIN_H
#define WOLWAKEUPPLUGIN_H

#include "wakeupplugin.h"

#include <QUdpSocket>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class WolWakeUpPlugin : public WakeUpPlugin
{
	Q_OBJECT

	QString macAddress_;

	int port_;

	QString destinationAddress_;

	QUdpSocket* socket_ = nullptr;

  public:
	explicit WolWakeUpPlugin(QObject* parent = nullptr);

	Q_PROPERTY(QString macAddress READ macAddress WRITE setMacAddress NOTIFY macAddressChanged)
	Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
	Q_PROPERTY(
	    QString destinationAddress READ destinationAddress WRITE setDestinationAddress NOTIFY destinationAddressChanged)

	// WakeUpPlugin interface
  public:
	QString name() const override;

	QString macAddress() const;

	int port() const;

	QString destinationAddress() const;

	static QString pluginName();

	static int defaultPort();

public slots:
	void setMacAddress(QString macAddress);

	void setPort(int port);

	void setDestinationAddress(QString destinationAddress);

signals:

	void macAddressChanged(QString macAddress);

	void portChanged(int port);

	void destinationAddressChanged(QString destinationAddress);

protected:
	void wakeUp_() override;
};

}
}
}

#endif // WOLWAKEUPPLUGIN_H
