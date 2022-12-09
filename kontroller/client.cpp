#include "client.h"

#include "applicationsettings.h"
#include "playerservice.h"

#include "plugins/volume/kodivolumeplugin.h"
#include "plugins/volume/minidspvolumeplugin.h"

#include "plugins/wakeup/wolwakeupplugin.h"

#include <QSettings>
#include <QAuthenticator>
#include <QNetworkReply>
#include <QNetworkInterface>

namespace eu::tgcm::kontroller
{

namespace
{

VolumePlugin* getVolumePlugin_(Client* owner, Server* server)
{
	if(server == nullptr)
		return nullptr;
	QString pluginName = server->volumePluginName();
	if(pluginName == KodiVolumePlugin::static_name())
		return new KodiVolumePlugin(owner);
	if(pluginName == MinidspVolumePlugin::static_name())
	{
		QString address = server->volumePluginParameters().value("address").toString();
		auto plugin = new MinidspVolumePlugin(owner);
		plugin->setIpAddress(address);
		return plugin;
	}
	return new KodiVolumePlugin(owner); // by default, return a kodi volume plugin. This, at least, is safe
}

WakeUpPlugin* getWakeUpPlugin(Client* owner, Server* server)
{
	if (server == nullptr)
		return nullptr;
	QString pluginName = server->wakeUpPluginName();
	if (pluginName == WolWakeUpPlugin::pluginName())
	{
		QString macAddress = server->wakeUpPluginParameters().value("macAddress").toString();
		QString ipAddress = server->wakeUpPluginParameters().value("ipAddress").toString();
		bool ok = false;
		int port = server->wakeUpPluginParameters().value("port").toInt(&ok);
		if (!ok)
			port = WolWakeUpPlugin::defaultPort();
		auto plugin = new WolWakeUpPlugin(owner);
		plugin->setMacAddress(macAddress);
		plugin->setDestinationAddress(ipAddress);
		plugin->setPort(port);
		return plugin;
	}
	return nullptr;
}

// helper function, for compatibility with old qt
bool isWifi_(QNetworkInterface const& interface)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
	return interface.type() == QNetworkInterface::Wifi;
#else
	return interface.name().startsWith("wlan");
#endif
}

bool isUp_(QNetworkInterface const& interface)
{
	return (interface.flags() & QNetworkInterface::IsUp) == QNetworkInterface::IsUp;
}

} // namespace

Client::Client(ApplicationSettings* settings, QObject* parent) :
    QObject(parent),
    settings_{settings},
    serverUuid_(),
    client_(nullptr),
    clientSocket_(nullptr),
    tcpClient_(nullptr),
    connectionStatus_(0),
    downloadService_{new DownloadService{this, settings}},
    playerService_{new PlayerService{this, this}},
    timer_{new QTimer(this)},
    timerNetworkInterface_{new QTimer(this)}
{
	// refresh the client if servers changes
	// because the server may disappear
	connect(settings, &ApplicationSettings::serversChanged, this, &Client::refresh);
	connect(timer_, &QTimer::timeout, this, &Client::pollNext_);
	connect(timerNetworkInterface_, &QTimer::timeout, this, &Client::checkWifiStatus_);
	timerNetworkInterface_->setSingleShot(false);
	timerNetworkInterface_->start(5000);
	checkWifiStatus_();
}

Client::~Client() noexcept
{
	freeConnections();
}

void Client::freeConnections()
{
	if(client_)
		client_->deleteLater();
	client_ = nullptr;
	if(clientSocket_)
		clientSocket_->deleteLater();
	clientSocket_ = nullptr;
	if(tcpClient_)
		tcpClient_->deleteLater();
	tcpClient_ = nullptr;
}

QString Client::serverAddress() const
{
	return server_->serverAddress();
}

int Client::serverPort() const
{
	return server_->serverPort();
}

int Client::serverHttpPort() const
{
	return server_->serverHttpPort();
}

void Client::refresh()
{
	freeConnections();
	setConnectionStatus_(ConnectionStatus::Connecting);
	server_ = nullptr;
	if(serverUuid_.size() > 0)
		server_ = settings_->server(serverUuid_);
	else
		server_ = settings_->server(settings_->lastServer());
	if(server_)
	{
		volumePlugin_ = getVolumePlugin_(this, server_);
		wakeUpPlugin_ = getWakeUpPlugin(this, server_);
		emit serverChanged();
		emit volumePluginChanged();
		emit wakeUpPluginChanged(wakeUpPlugin_);
		serverUuid_ = server_->uuid();
		qDebug() << "Connection to " << server_->serverAddress() << server_->serverPort();
		if (wifiUp() || server_->ignoreWifiStatus())
		{
			if (server_->serverAddress().size() > 0 && server_->serverPort() > 0)
			{
				setConnectionStatus_(ConnectionStatus::Connecting);
				{
					clientSocket_ = new QTcpSocket();
					connect(clientSocket_, SIGNAL(connected()), this, SLOT(handleConnectionSuccess_()));
					connect(clientSocket_,
					        SIGNAL(error(QAbstractSocket::SocketError)),
					        this,
					        SLOT(handleConnectionError_(QAbstractSocket::SocketError)));
					clientSocket_->connectToHost(server_->serverAddress(), static_cast<quint16>(server_->serverPort()));
				}
			}
		}
		else
		{
			qDebug() << "Not connecting, wifi down and not ignoring it";
			setConnectionStatus_(ConnectionStatus::NoWifi);
		}
	}
	else
		setConnectionStatus_(ConnectionStatus::ConnectionError);
}

int Client::connectionStatus() const
{
	return connectionStatus_;
}

bool Client::wifiUp() const
{
	return wifiUp_;
}

bool Client::useHttpInterface() const
{
	return false;
}

Server *Client::server()
{
	return server_;
}

void Client::switchToServer(const QString &serverUuid)
{
	if(serverUuid != serverUuid_)
	{
		serverUuid_ = serverUuid;
		emit serverChanged();
		refresh();
	}
}

DownloadService* Client::downloadService() const
{
	return downloadService_;
}

PlayerService* Client::playerService() const
{
	return playerService_;
}

void Client::handleError(QJsonRpcMessage error)
{
	if(error.errorCode() == QJsonRpc::ErrorCode::TimeoutError)
	{
		refresh(); // refresh the connection, this is an error
	}
	else if(error.errorMessage().startsWith("error with http request"))
	{
		setConnectionStatus_(ConnectionStatus::ConnectionError);
	}
	else
	{
		qDebug() << error;
		qDebug() << error.errorMessage();
	}
}

QJsonRpcServiceReply* Client::send(QJsonRpcMessage message)
{
	if(connectionStatus_ <= 0)
	{
		refresh();
	}
	if(tcpClient_)
	{
		/*        auto reply = tcpClient_->sendMessage(message);
		connect(reply, SIGNAL(finished()), this, SLOT(handleReplyFinished()));
		return reply; */
		return httpSend(message);
	}
	else
		return nullptr;
}

QJsonRpcServiceReply *Client::httpSend(QJsonRpcMessage message)
{
	auto reply = client()->sendMessage(message);
	connect(reply, SIGNAL(finished()), this, SLOT(handleReplyFinished_()));
	return reply;
}

QString Client::baseUrl() const
{
	return "http://" + serverAddress() + ":" + QString::number(serverHttpPort()) + "/";
}

QNetworkReply* Client::downloadFile(QString path)
{
	QNetworkRequest request;
	request.setUrl(baseUrl() + path);
	auto reply = client()->networkAccessManager()->get(request);
	return reply;
}

void Client::setDownloadService(DownloadService* downloadService)
{
	if (downloadService_ == downloadService)
		return;

	downloadService_ = downloadService;
	emit downloadServiceChanged(downloadService_);
}

void Client::retryConnect()
{
	refresh();
}

void Client::setPlayerService(PlayerService* playerService)
{
	if (playerService_ == playerService)
		return;

	playerService_ = playerService;
	emit playerServiceChanged(playerService_);
}

void Client::wakeUp()
{
	if (wakeUpPlugin_ != nullptr)
		wakeUpPlugin_->wakeUp();
}

void Client::handleReplyFinished_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply)
	{
		auto response = reply->response();
		if(response.type() == QJsonRpcMessage::Error)
		{
			qDebug() << reply->response();
			handleError(reply->response());
		}
		reply->deleteLater();
	}
	else
	{
		setConnectionStatus_(ConnectionStatus::ConnectionError);
		sender()->deleteLater();
	}
}

void Client::setConnectionStatus_(ConnectionStatus connectionStatus)
{
	connectionStatus_ = static_cast<int>(connectionStatus);
	emit connectionStatusChanged(connectionStatus_);
}

void Client::handleConnectionSuccess_()
{
	tcpClient_ = new QJsonRpcSocket(clientSocket_);
	connect(tcpClient_, SIGNAL(messageReceived(QJsonRpcMessage)), this,
	        SLOT(handleMessageReceived_(QJsonRpcMessage)));
	setConnectionStatus_(ConnectionStatus::Connected);
	playerService_->refreshPlayerInfo();
	volumePlugin()->refreshVolume();
	refreshServerParameters_();
	emit serverChanged();
}

void Client::handleConnectionError_(QAbstractSocket::SocketError err)
{
	setConnectionStatus_(ConnectionStatus::ConnectionError);
	if(client_ != nullptr)
	{
		auto mess = QJsonRpcMessage::createRequest("JSONRPC.Ping");
		auto query = client_->sendMessage(mess);
		connect(query, &QJsonRpcServiceReply::finished, this, &Client::handlePingReply_);
	}
	qDebug() << err;
}

void Client::handlePingReply_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply != nullptr)
	{
		if (reply->response().type() == QJsonRpcMessage::Type::Response)
		{
			qDebug() << "Ping successful";
			setConnectionStatus_(ConnectionStatus::Connected);
			playerService_->refreshPlayerInfo();
			volumePlugin()->refreshVolume();
			refreshServerParameters_();
			emit serverChanged();
			setupPolling_();
		}
		else
			setConnectionStatus_(ConnectionStatus::ConnectionError);
		reply->deleteLater();
	}
	else
		sender()->deleteLater();
}

void Client::handleMessageReceived_(QJsonRpcMessage message)
{
	if(message.type() == QJsonRpcMessage::Notification)
	{
		QString method = message.method();
		if(method == "Player.OnPause" || method == "Player.OnPlay" || method == "Player.OnResume")
		{
			QJsonObject data = message.params().toObject().value("data").toObject();
			QJsonValue player = data.value("player");
			int playerId;
			int speed;
			int itemId = -1;
			if(!player.isObject())
				return;
			auto playerIdVal = player.toObject().value("playerid");
			if(!playerIdVal.isDouble())
				return;
			playerId = static_cast<int>(playerIdVal.toDouble());
			auto speedVal = player.toObject().value("speed");
			if(!speedVal.isDouble())
				return;
			speed = static_cast<int>(speedVal.toDouble());
			emit playerSpeedChanged(playerId, speed);
			QJsonObject item = data.value("item").toObject();
			QJsonValue id = item.value("id");
			QString type = item.value("type").toString();
			QString file = item.value("file").toString();
			if(id.isDouble())
				itemId = static_cast<int>(id.toDouble());
			emit playlistCurrentItemChanged(playerId, type, itemId, file);
		}
		else if(method == "Player.OnStop")
		{
			QJsonObject data = message.params().toObject().value("data").toObject();
			QJsonValue item = data.value("item");
			int playerId = -1;
			if (item.isObject())
			{
				auto pId = item.toObject().value("id");
				if (pId.isDouble())
					playerId = pId.toInt();
			}
			emit playerStopped(playerId);
		}
		else if(method == "Playlist.OnClear")
		{
			QJsonValue val = message.params().toObject().value("data").toObject().value("playlistid");
			if(val.isDouble())
			{
				emit playlistCleared(static_cast<int>(val.toDouble()));
			}
		}
		else if(method == "Playlist.OnRemove")
		{
			QJsonObject data = message.params().toObject().value("data").toObject();
			if(!data.isEmpty())
			{
				QJsonValue playlistId = data.value("playlistid");
				QJsonValue position = data.value("position");
				if(playlistId.isDouble() && position.isDouble())
					emit playlistElementRemoved(
				            static_cast<int>(playlistId.toDouble()),
				            static_cast<int>(position.toDouble()));
			}
		}
		else if(method == "Playlist.OnAdd")
		{
			QJsonObject data = message.params().toObject().value("data").toObject();
			if(!data.isEmpty())
			{
				QJsonValue playlistId = data.value("playlistid");
				if(playlistId.isDouble())
					emit playlistElementAdded(static_cast<int>(playlistId.toDouble()));
			}
		}
		else if(method == "Player.OnSeek")
		{
			QJsonObject data = message.params().toObject().value("data").toObject();
			if(!data.isEmpty())
			{
				QJsonObject player = data.value("player").toObject();
				if(!player.isEmpty())
				{
					int playerId = static_cast<int>(player.value("playerId").toDouble());
					QJsonObject offset = player.value("seekoffset").toObject();
					if(!offset.isEmpty())
					{
						int hours = static_cast<int>(offset.value("hours").toDouble());
						int minutes = static_cast<int>(offset.value("minutes").toDouble());
						int seconds = static_cast<int>(offset.value("seconds").toDouble());
						int milliseconds = static_cast<int>(offset.value("milliseconds").toDouble());
						emit playerSeekChanged(playerId, hours, minutes, seconds, milliseconds);
					}
				}
			}
		}
		else if (method == "Player.OnPropertyChanged")
		{
			QJsonObject data = message.params().toObject().value("data").toObject();
			if (!data.isEmpty())
			{
				QJsonObject player = data.value("player").toObject();
				if (!player.isEmpty())
				{
					int playerId = static_cast<int>(player.value("playerId").toDouble());
					for (auto& player : playerService_->playersList())
					{
						if (player->playerId() == playerId)
						{
							player->updateProperty(data.value("property").toObject());
						}
					}
				}
			}
		}
		else if (method == "Application.OnVolumeChanged")
		{
			QJsonObject data = message.params().toObject().value("data").toObject();
			if (!data.isEmpty())
			{
				// try to cast into kodivolumeplugin. Will succeed if the volume plugin handles this event
				auto kodiPlugin = dynamic_cast<KodiVolumePlugin*>(volumePlugin_);
				if(kodiPlugin != nullptr)
				{
					kodiPlugin->interpretOnVolumeChanged(data);
				}
			}
		}
		else if(method == "Input.OnInputRequested")
		{
			auto data = message.params().toObject().take("data").toObject();
			emit inputRequested(data["title"].toString(),
			        data["type"].toString(),
			        data["value"].toString());
		}
		else if(method == "Input.OnInputFinished")
		{
			emit inputFinished();
		}
		else if (method == "VideoLibrary.OnScanStarted")
		{
			emit videoLibraryScanStarted();
		}
		else if (method == "VideoLibrary.OnScanFinished")
		{
			emit videoLibraryScanFinished();
		}
		else if (method == "AudioLibrary.OnScanStarted")
		{
			emit audioLibraryScanStarted();
		}
		else if (method == "AudioLibrary.OnScanFinished")
		{
			emit audioLibraryScanFinished();
		}
		else if (method == "VideoLibrary.OnCleanStarted")
		{
			emit videoLibraryCleanStarted();
		}
		else if (method == "VideoLibrary.OnCleanFinished")
		{
			emit videoLibraryCleanFinished();
		}
		else if (method == "AudioLibrary.OnCleanStarted")
		{
			emit audioLibraryCleanStarted();
		}
		else if (method == "AudioLibrary.OnCleanFinished")
		{
			emit audioLibraryCleanFinished();
		}
		else
			qDebug() << message;
	}
}

void Client::provideCredentials_(QNetworkReply * /*reply*/, QAuthenticator *authenticator)
{
	authenticator->setUser(server_->login());
	authenticator->setPassword(server_->password());
}

void Client::refreshServerParameters_()
{
	QJsonObject parameters;
	parameters.insert("level", "standard");
	QJsonObject filter;
	filter.insert("section", "media");
	filter.insert("category", "filelists");
	parameters.insert("filter", filter);
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Settings.GetSettings", parameters);
	auto reply = send(message);
	if (reply)
	{
		connect(reply, &QJsonRpcServiceReply::finished, this, &Client::handleRefreshServerParameters_);
	}
}

void Client::handleRefreshServerParameters_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply != nullptr)
	{
		auto const response = reply->response().toObject();
		auto result = response.find("result");
		if (result != response.end() && result->type() == QJsonValue::Object)
		{
			auto const r = result->toObject();
			auto settings = r.find("settings");
			if (settings != r.end() && settings->type() == QJsonValue::Array)
			{
				auto const arr = settings->toArray();
				for (auto const& setting : arr)
				{
					if (!(setting.type() == QJsonValue::Object))
						continue; // ignore non objects
					auto const s = setting.toObject();
					QString id = s["id"].toString();
					if (id == "filelists.ignorethewhensorting")
					{
						auto const value = s["value"];
						if (value.type() == QJsonValue::Bool)
						{
							sortIgnoreArticle_ = value.toBool();
						}
					}
				}
			}
		}
	}
}

void Client::pollNext_()
{
	// do the actual calls
}

void Client::checkWifiStatus_()
{
	auto list = QNetworkInterface::allInterfaces();
	for (auto& interface : list)
	{
		if (isWifi_(interface))
		{
			if (!isUp_(interface) && wifiUp_)
			{
				wifiUp_ = false;
				emit wifiUpChanged(false);
				if (server() && !server()->ignoreWifiStatus())
				{
					setConnectionStatus_(ConnectionStatus::NoWifi);
				}
			}
			else if (isUp_(interface) && !wifiUp_)
			{
				wifiUp_ = true;
				emit wifiUpChanged(true);
				if (connectionStatus_ == -2 && server() != nullptr)
				{
					refresh(); // retry connection, wifi is up !
				}
			}
		}
	}
}

QJsonRpcHttpClient* Client::client()
{
	if (!client_)
	{
		client_ = new QJsonRpcHttpClient(baseUrl() + "jsonrpc");
		connect(client_->networkAccessManager(),
		        &QNetworkAccessManager::authenticationRequired,
		        this,
		        &Client::provideCredentials_);
	}
	return client_;
}

void Client::setupPolling_()
{
	timer_->setInterval(1000);
	timer_->setSingleShot(false);
	timer_->start();
}

VolumePlugin* Client::volumePlugin()
{
	return volumePlugin_;
}

bool Client::sortIgnoreArticle() const
{
	return sortIgnoreArticle_;
}

WakeUpPlugin* Client::wakeUpPlugin() const
{
	return wakeUpPlugin_;
}

} // namespace eu::tgcm::kontroller
