#ifndef EU_TGCM_KONTROLLER_STATUSSERVICE_H
#define EU_TGCM_KONTROLLER_STATUSSERVICE_H

#include <QObject>
#include <QTimer>
#include <QNetworkConfigurationManager>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class ApplicationSettings;
class Client;

class StatusService : public QObject
{
	Q_OBJECT
	QNetworkConfigurationManager manager_;
	bool wifiEnabled_ = false;
public:
	StatusService(QObject* parent = nullptr);

	Q_PROPERTY(eu::tgcm::kontroller::ApplicationSettings* settings READ settings WRITE setSettings NOTIFY settingsChanged)
	Q_PROPERTY(bool wifiEnabled READ wifiEnabled WRITE setWifiEnabled NOTIFY wifiEnabledChanged)

	int connectionStatus() const;

	bool wifiEnabled() const;

	eu::tgcm::kontroller::ApplicationSettings* settings() const;

public slots:

	void setSettings(eu::tgcm::kontroller::ApplicationSettings* settings);

private:
	bool atLeastOneWifiConnected();

	eu::tgcm::kontroller::ApplicationSettings* settings_ = nullptr;

signals:
	bool wifiEnabledChanged(bool);

	void clientChanged(eu::tgcm::kontroller::Client* client);

	void settingsChanged(eu::tgcm::kontroller::ApplicationSettings* settings);

private slots:
	void setWifiEnabled(bool wifi);
	void handleConnectionAdded_(QNetworkConfiguration const& config);
	void handleConnectionRemoved_(QNetworkConfiguration const& config);
	void handleConnectionChanged_(QNetworkConfiguration const& config);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_STATUSSERVICE_H
