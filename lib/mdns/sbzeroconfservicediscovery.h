#ifndef SBZEROCONFSERVICEDISCOVERY_H
#define SBZEROCONFSERVICEDISCOVERY_H

#include <QNetworkInterface>
#include <QObject>

#include <chrono>

#include "sbzeroconfservicerecord.h"

class SBZeroConfServiceDiscoveryPrivate;

class SBZeroConfServiceDiscovery : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(SBZeroConfServiceDiscovery)

	QScopedPointer<SBZeroConfServiceDiscoveryPrivate> const d_ptr;

  public:
	SBZeroConfServiceDiscovery(QObject* parent = nullptr);

	~SBZeroConfServiceDiscovery() noexcept;

	void setInterface(QNetworkInterface interface);

	void discover(std::chrono::milliseconds timeout);

	void query(QString serviceName, std::chrono::milliseconds timeout);

	/**
	 * @brief interruptCurrentOperation requests immediate finish of the current operation.
	 * This will still result in a discoveryFinised signal being sent iff it was not
	 * already sent
	 */
	void interruptCurrentOperation();

  signals:
	void serviceDiscovered(SBZeroConfServiceRecord);
	void discoveryFinished();
	/**
	 * @brief discoveryError signal sent when a fatal error occurs. In that case, no
	 * discoveryFinished signal will be sent
	 * @param message A message describing the error
	 */
	void discoveryError(QString message);

  private slots:
	void parseData_();
	void finishDiscovery_();

};

#endif // SBZEROCONFSERVICEDISCOVERY_H
