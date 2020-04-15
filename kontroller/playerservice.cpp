#include "playerservice.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlayerService::PlayerService(eu::tgcm::kontroller::Client* client, QObject *parent) : QObject(parent),
    client_{client}
{
	refreshTimer_.setInterval(5000);
	refreshTimer_.setSingleShot(false);
	playerRefreshTimer_.setInterval(200);
	playerRefreshTimer_.setSingleShot(true);
	connect(&refreshTimer_, &QTimer::timeout, this, &PlayerService::refreshPlayerInfo);
	connect(&playerRefreshTimer_, &QTimer::timeout, this, &PlayerService::refreshPlayerInfo);
	connect(client_, &Client::connectionStatusChanged, this, &PlayerService::updateConnectionStatus_);
	connect(client_, &Client::playerStopped, this, &PlayerService::stopPlayer_);
	connect(client_, &Client::playerSpeedChanged, this, &PlayerService::updatePlayerSpeed);
	connect(client_, &Client::playerSeekChanged, this, &PlayerService::updatePlayerSeek_);
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

Player *PlayerService::getPlayer(QString type)
{
    for(Player* player : currentPlayers_)
    {
        if(player->type() == type)
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
	QVector<bool> playerStillExistings; // stores a list of players still existing. Deleting and recreating
	                                        // players cause a glitch, so we try to update them instead
	for(int i = 0; i < currentPlayers_.size(); ++i)
		playerStillExistings.push_back(false);
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply)
	{
		QJsonRpcMessage const& response = reply->response();
		if(response.errorCode() == 0)
		{
			QJsonValue val = response.result();
			if(val.isArray())
			{
				QJsonArray arr = val.toArray();
				if(arr.size() != 0)
				{
					for(int i = 0; i < arr.size(); ++i)
					{
						QJsonValue val2 = arr[i];
						if(val2.isObject())
						{
							// find wether this player already existed :
							Player* player = nullptr;
							auto playerId = val2.toObject()["playerid"].toInt();
							for(int i = 0; i < playerStillExistings.size(); ++i)
							{
								if(currentPlayers_[i]->playerId() == playerId)
								{
									playerStillExistings[i] = true;
									player = currentPlayers_[i];
								}
							}
							if(player == nullptr)
							{
								currentPlayers_.push_back(new Player(client_, this));
								player = currentPlayers_.back();
							}
							player->setPlayerId(val2.toObject()["playerid"].toInt());
							player->setType(val2.toObject()["type"].toString());
							player->refreshPlayerStatus();
						}
					}
				}
			}
		}
	}
	auto itPlayers = currentPlayers_.begin();
	auto itExists = playerStillExistings.begin();
	while(itExists != playerStillExistings.end())
	{
		if(!*itExists)
		{
			itExists = playerStillExistings.erase(itExists);
			(*itPlayers)->deleteLater();
			itPlayers = currentPlayers_.erase(itPlayers);
		}
		else
		{
			++itExists;
			++itPlayers;
		}
	}
	emit playersChanged();
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
		currentPlayers_.clear();
		emit playersChanged();
	}
}

void PlayerService::updatePlayerSpeed(int playerId, int speed)
{
    bool found = false;
    for(Player* player : currentPlayers_)
    {
        if(player->playerId() == playerId)
        {
            found = true;
            player->setSpeed(speed);
        }
    }
    if(!found)
        refreshPlayerInfo();
}

void PlayerService::stopPlayer_()
{
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
			player->setTime(player->time() +
                        hours * 3600 * 1000 +
                            minutes * 60 * 1000 +
                            seconds * 1000 +
                            milliseconds
                        );
        }
    }
    if(!found)
        refreshPlayerInfo();
/*    else
		emit playersChanged(); */
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

}
}
}
