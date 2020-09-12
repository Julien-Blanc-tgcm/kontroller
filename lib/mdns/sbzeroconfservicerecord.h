#ifndef SBZEROCONFSERVICERECORD_H
#define SBZEROCONFSERVICERECORD_H

#include <QHostAddress>
#include <QObject>

class SBZeroConfServiceRecord
{
	Q_GADGET

  public:
	SBZeroConfServiceRecord();

	Q_PROPERTY(QString service READ service WRITE setService)
	Q_PROPERTY(QString hostname READ hostname WRITE setHostname)
	Q_PROPERTY(QString address READ address WRITE setAddress)
	Q_PROPERTY(int port READ port WRITE setPort)
	QString service() const;

	QString hostname() const;

	QString address() const;

	int port() const;

  public slots:
	void setService(QString service);

	void setHostname(QString hostname);

	void setAddress(QString address);

	void setPort(int port);

  private:
	/**
	 * @brief hostAddress_ stores the host address as a QHostAddress, event if properties use strings
	 */
	QHostAddress hostAddress_;
	QString service_;
	QString hostname_;
	int port_;
};

#endif // SBZEROCONFSERVICERECORD_H
