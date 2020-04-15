#ifndef MINIDSPVOLUMEPLUGIN_H
#define MINIDSPVOLUMEPLUGIN_H

#include "volumeplugin.h"

#include <minidsplib/query.h>

#include <QVector>

class QTcpSocket;
class QTimer;

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class Client;
/**
 * @brief The VolumePlugin class is the base class for all volume plugins. Volume plugins shall provide a set of method to query
 * the current volume, the min/max volumes and the volume step (ie, what a button press should increment)
 *
 * @note The volume exposed is in the range [0-255]. The minidsp lib use negative volumes, in the range
 * [-255, 0]. The offset is applied for all functions of this volume plugin.
 */
class MinidspVolumePlugin : public VolumePlugin
{
	Q_OBJECT
public:
	explicit MinidspVolumePlugin(Client *parent);

	Q_PROPERTY(QString ipAddress READ ipAddress WRITE setIpAddress NOTIFY ipAddressChanged)
	Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)

	QString ipAddress() const;

	bool connected() const;

	static QString static_name();

public slots:
	void setIpAddress(QString ipaddress);

	void setConnected(bool connected);

signals:
	void ipAddressChanged(QString ipaddress);

	void connectedChanged(bool connected);

protected:
	QString realName_() const override;
	int maxVolume_() const override;
	int minVolume_() const override;
	int currentVolume_() const override;
	void updateVolume_(int value) override;
	int volumeStep_() const override;
	void refreshVolume_() override;

private:
	int currentVolumeStored_ = -1;

	QString ipAddress_;

	bool connected_ = false;

	bool muted_ = false;

	/**
	 * @brief timer_ connection timer, used to not hold the connection for too long. Since the device responds only
	 * to one connection at a time, we don't want to hold it.
	 */
	QTimer* timer_ = nullptr;
	QTcpSocket* socket_ = nullptr;

	QVector<minidsp::Query::Message> pendingQueries_;

	void pushPendingQuery_(minidsp::Query::Message const& query);
	void executeNextQuery_();

	void executeQuery_(minidsp::Query::Message const& query);

private slots:
	void handleConnected_();
	void handleDisconnected_();
	void handleReply_();

};

}
}
}

#endif // VOLUMEPLUGIN_H
