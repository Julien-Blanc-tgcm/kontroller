#include "playerservice.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlayerService::PlayerService(eu::tgcm::kontroller::Client* client, QObject* parent) : QObject(parent), client_{client}
{
	refreshTimer_.setInterval(5000);
	refreshTimer_.setSingleShot(false);
	playerRefreshTimer_.setInterval(300);
	playerRefreshTimer_.setSingleShot(true);
	connect(&refreshTimer_, &QTimer::timeout, this, &PlayerService::refreshPlayerInfo);
	connect(&playerRefreshTimer_, &QTimer::timeout, this, &PlayerService::refreshPlayerInfo);
	connect(client_, &Client::connectionStatusChanged, this, &PlayerService::updateConnectionStatus_);
	connect(client_, &Client::playerStopped, this, &PlayerService::stopPlayer_);
	connect(client_, &Client::playerSpeedChanged, this, &PlayerService::updatePlayerSpeed);
	connect(client_, &Client::playerSeekChanged, this, &PlayerService::updatePlayerSeek_);
	// hard code the three players. They are hard-coded in kodi anyway, and there is no interface
	// to retrieve them if they are not running
	for (int i = 0; i <= 2; ++i)
	{
		auto player = new Player(client, i, this);
		currentPlayers_.push_back(player);
	}
}

void PlayerService::refreshPlayerInfo()
{
	if(!refreshPending_)
	{
		refreshPending_ = true;
		QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.getActivePlayers");
		QJsonRpcServiceReply* reply = client_->send(message);
		if(reply)
			connect(reply, &QJsonRpcServiceReply::finished, this, &PlayerService::refreshPlayerInfoCb_);
		else
			refreshPending_ = false;
	}
}

void PlayerService::playPause(int playerId)
{
	if(playerId < currentPlayers_.size())
		currentPlayers_[playerId]->playPause();
}

const QList<Player*>& PlayerService::playersList()
{
	return currentPlayers_;
}

int PlayerService::getPlayerId(QString type)
{
	for(Player* player : currentPlayers_)
	{
		if(player->type() == type)
			return player->playerId();
	}
	return -1;
}

Player* PlayerService::getPlayer(QString type)
{
	for (Player* player : currentPlayers_)
	{
		if (player->type() == type)
			return player;
	}
	return nullptr;
}

QQmlListProperty<Player> PlayerService::players()
{
	return QQmlListProperty<Player>(this, this, &PlayerService::playerCount_, &PlayerService::playerAt_);
}

void PlayerService::refreshPlayerInfoCb_()
{
	auto oldActive = activePlayer();
	QVector<int> playerStillPlaying;
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply)
	{
		QJsonRpcMessage const& response = reply->response();
		if(response.errorCode() == 0)
		{
			QJsonValue val = response.result();
			if(val.isArray())
			{
				QJsonArray const arr = val.toArray();
				if(!arr.empty())
				{
					for(auto const& val2 : arr)
					{
						if(val2.isObject())
						{
							// find wether this player already existed :
							auto playerId = val2.toObject()["playerid"].toInt();
							for (auto p : currentPlayers_)
							{
								if (p->playerId() == playerId)
								{
									p->setActive(true);
									p->refreshPlayerStatus();
									break;
								}
							}
							playerStillPlaying.push_back(playerId);
						}
					}
				}
			}
		}
	}
	for (auto p : currentPlayers_)
	{
		if (!playerStillPlaying.contains(p->playerId()))
			p->setActive(false);
	}
	auto newActive = activePlayer();
	if (newActive != oldActive)
		emit activePlayerChanged();
	refreshPending_ = false;
}

void PlayerService::updateConnectionStatus_(int newStatus)
{
	if(newStatus == 2)
	{
		refreshPlayerInfo();
		if(client_->useHttpInterface())
			refreshTimer_.start();
		else
			refreshTimer_.stop();
	}
	else if(newStatus == 0)
	{
		// TODO: indicate a problem
	}
}

void PlayerService::updatePlayerSpeed(int playerId, int speed)
{
	bool found = false;
	for (Player* player : currentPlayers_)
	{
		if (player->playerId() == playerId)
		{
			found = true;
			if(speed == 0)
			{
				player->setSpeed(speed);
			}
			else
			{
				player->setActive(true);
				player->refreshPlayerStatus();
				emit activePlayerChanged();
			}
		}
	}
	if(!found)
		refreshPlayerInfo();
}

Player* PlayerService::activePlayer()
{
	for(auto p : currentPlayers_)
	{
		if (p->active())
			return p;
	}
	return nullptr;
}

void PlayerService::stopPlayer_(int playerId)
{
	for (auto player : currentPlayers_)
	{
		if (player->playerId() == playerId)
		{
			player->refreshPlayerStatus();
		}
	}
	// to delete the player, since it is no longer active
	playerRefreshTimer_.start();
}

void PlayerService::updatePlayerSeek_(int playerId, int hours, int minutes, int seconds, int milliseconds)
{
	bool found = false;
	for(Player* player : currentPlayers_)
	{
		if(player->playerId() == playerId)
		{
			found = true;
			player->setTime(player->time() + hours * 3600 * 1000 + minutes * 60 * 1000 + seconds * 1000 + milliseconds);
		}
	}
	if(!found)
		refreshPlayerInfo();
}

int PlayerService::playerCount_(QQmlListProperty<Player>* list)
{
	return static_cast<PlayerService*>(list->data)->currentPlayers_.size();
}

Player* PlayerService::playerAt_(QQmlListProperty<Player>* list, int index)
{
	auto& l = static_cast<PlayerService*>(list->data)->currentPlayers_;
	if(index < l.size())
		return l[index];
	return nullptr;
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
