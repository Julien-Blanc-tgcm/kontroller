#ifndef EU_TGCM_KONTROLLER_CLIENT_H
#define EU_TGCM_KONTROLLER_CLIENT_H

#include "downloadservice.h"

#include <qjsonrpc/qjsonrpchttpclient.h>

#include <QTimer>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class ApplicationSettings;
class PlayerService;
class Server;
class VolumePlugin;
class WakeUpPlugin;

class Client : public QObject
{
	Q_OBJECT
  public:
	enum ConnectionStatus
	{
		NoWifi = -2,
		ConnectionError = -1,
		Unconnected = 0,
		Connecting = 1,
		Connected = 2
	};

	Q_ENUM(ConnectionStatus)

  private:
	ApplicationSettings* settings_;
	QString serverUuid_;
	QJsonRpcHttpClient* client_;
	QTcpSocket* clientSocket_;
	QJsonRpcSocket* tcpClient_;
	/**
	 * @see connectionStatus()
	 */
	ConnectionStatus connectionStatus_;

	Server* server_ = nullptr;

	/**
	 * @brief httpOnly_ will not make use of the tcp connection to receive notifications, but only the http. Thus, will
	 * use polling for some data
	 */
	bool httpOnly_ = false;

	void freeConnections();
	eu::tgcm::kontroller::DownloadService* downloadService_ = nullptr;

	eu::tgcm::kontroller::PlayerService* playerService_ = nullptr;

	VolumePlugin* volumePlugin_ = nullptr;

	bool sortIgnoreArticle_ = false;

	eu::tgcm::kontroller::WakeUpPlugin* wakeUpPlugin_ = nullptr;

	QTimer* timer_ = nullptr;

	/**
	 * @brief timerNetworkInterface_ uses polling to check the state of the network interfaces.
	 * 5s polling is enough
	 */
	QTimer* timerNetworkInterface_ = nullptr;

	bool wifiUp_ = false;

public:
	explicit Client(ApplicationSettings* settings, QObject *parent = nullptr);
	~Client() noexcept override;
	Client(Client&&) = delete;
	Client(Client const&) = delete;
	Client& operator=(Client&&) = delete;
	Client& operator=(Client const&) = delete;

	QString serverAddress() const;

	int serverPort() const;

	int serverHttpPort() const;

	/**
	 * @brief connectionStatus tells whether the client is connected
	 * -2 means no wifi and it is required
	 * -1 means connection error
	 * 0 means unconnected
	 * 1 means connecting
	 * 2 means connected
	 * @return
	 */
	ConnectionStatus connectionStatus() const;

	/**
	 * @brief wifiUp returns the state of the wifi connection
	 * @return true / false
	 */
	bool wifiUp() const;

	bool useHttpInterface() const;

	Server* server();

	Q_PROPERTY(eu::tgcm::kontroller::DownloadService* downloadService READ downloadService WRITE setDownloadService \
	           NOTIFY downloadServiceChanged)
	Q_PROPERTY(eu::tgcm::kontroller::Server* server READ server NOTIFY serverChanged)
	eu::tgcm::kontroller::DownloadService* downloadService() const;
	Q_PROPERTY(eu::tgcm::kontroller::Client::ConnectionStatus connectionStatus READ connectionStatus NOTIFY
	               connectionStatusChanged)

	Q_PROPERTY(eu::tgcm::kontroller::PlayerService* playerService READ playerService WRITE setPlayerService \
	           NOTIFY playerServiceChanged)

	Q_PROPERTY(eu::tgcm::kontroller::VolumePlugin* volumePlugin READ volumePlugin NOTIFY volumePluginChanged)

	Q_PROPERTY(bool sortIgnoreArticle READ sortIgnoreArticle NOTIFY sortIgnoreArticleChanged)

	Q_PROPERTY(eu::tgcm::kontroller::WakeUpPlugin* wakeUpPlugin READ wakeUpPlugin NOTIFY wakeUpPluginChanged)

	Q_PROPERTY(bool wifiUp READ wifiUp NOTIFY wifiUpChanged);

	eu::tgcm::kontroller::PlayerService* playerService() const;

	VolumePlugin* volumePlugin();

	bool sortIgnoreArticle() const;

	eu::tgcm::kontroller::WakeUpPlugin* wakeUpPlugin() const;

  signals:
	void connectionStatusChanged(eu::tgcm::kontroller::Client::ConnectionStatus connected);
	void serverChanged();
	void inputRequested(QString title, QString type, QString value);
	void inputFinished();
	void volumePluginChanged();

  public slots:
	void switchToServer(QString const& serverUuid);
	void refresh();
	void handleError(QJsonRpcMessage error);
	/**
	 * @brief send returns an object that can be used to wait for the query result. The kodi client will take care of releasing it
	 * @param message the json rpc query.
	 * @return
	 */
	QJsonRpcServiceReply* send(QJsonRpcMessage message);

	/**
	 * @brief httpSend does the same as send, but uses http transport. This is needed for ressources like images or files
	 * @param message
	 * @return
	 */
	QJsonRpcServiceReply* httpSend(QJsonRpcMessage message);

	/**
	 * @brief baseUrl returns the base url of the server, without the
	 * "/jsonrpc". May be used to compute some paths
	 * @return
	 */
	QString baseUrl() const;

	/**
	 * @brief downloadFile downloads a file
	 * @param path the path to download
	 * @return the network reply associated with this download
	 */
	QNetworkReply* downloadFile(QString path);

	void setDownloadService(eu::tgcm::kontroller::DownloadService* downloadService);

	void retryConnect();
	void setPlayerService(eu::tgcm::kontroller::PlayerService* playerService);

	/**
	 * @brief wakeUp try to wake up the remote device
	 */
	void wakeUp();

private slots:
	void handleReplyFinished_();

	void handleConnectionSuccess_();
	void handleConnectionError_(QAbstractSocket::SocketError);
	void handlePingReply_();
	void handleMessageReceived_(QJsonRpcMessage message);
	void provideCredentials_(QNetworkReply *reply, QAuthenticator *authenticator);
	void refreshServerParameters_();
	void handleRefreshServerParameters_();
	void pollNext_();
	void checkWifiStatus_();

  private:
	void setConnectionStatus_(ConnectionStatus connectionStatus);
	QJsonRpcHttpClient* client();
	void setupPolling_();

  signals:
	// these ones are the notifications the kodi api can send
	// note that if using HTTP transport only, no notifications will be available
	void playerSpeedChanged(int playerid, int speed);
	void playlistCurrentItemChanged(int playerid, QString type, int id, QString file);
	void playerStopped(int playerId);

	void playlistCleared(int playlistId);
	void playlistElementRemoved(int playlistId, int position);
	void playlistElementAdded(int playlistId);
	void playerSeekChanged(int playerId, int hours, int minutes, int seconds, int milliseconds);
	void downloadServiceChanged(eu::tgcm::kontroller::DownloadService* downloadService);
	void playerServiceChanged(eu::tgcm::kontroller::PlayerService* playerService);
	void sortIgnoreArticleChanged(bool sortIgnoreArticle);

	void videoLibraryScanStarted();
	void videoLibraryScanFinished();

	void audioLibraryScanStarted();
	void audioLibraryScanFinished();

	void videoLibraryCleanStarted();
	void videoLibraryCleanFinished();

	void audioLibraryCleanStarted();
	void audioLibraryCleanFinished();
	void wakeUpPluginChanged(eu::tgcm::kontroller::WakeUpPlugin* wakeUpPlugin);
	void wifiUpChanged(bool);
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu
#endif // EU_TGCM_KONTROLLER_CLIENT_H
