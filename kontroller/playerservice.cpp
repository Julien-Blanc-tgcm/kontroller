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
	connect(client_, &Client::connectionStatusChanged, this, &PlayerService::updateConnectionStatus);
	connect(client_, &Client::playerStopped, this, &PlayerService::stopPlayer_);
	connect(client_, &Client::playerSpeedChanged, this, &PlayerService::updatePlayerSpeed);
	connect(client_, &Client::playerSeekChanged, this, &PlayerService::updatePlayerSeek_);

	refreshPlayerInfo();
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
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.PlayPause", parameters);
	QJsonRpcServiceReply* reply = client_->send(message);
    if(reply)
        connect(reply, &QJsonRpcServiceReply::finished, this, &PlayerService::updatePlayPause_);
}

const QList<Player *> &PlayerService::players()
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
								currentPlayers_.push_back(new Player(this));
								player = currentPlayers_.back();
							}
							player->setPlayerId(val2.toObject()["playerid"].toInt());
							player->setType(val2.toObject()["type"].toString());
							refreshPlayerStatus(player->playerId());
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

namespace {

int getTime(QJsonObject time)
{
    auto hours = time.value("hours").toDouble();
    auto minutes = time.value("minutes").toDouble();
    auto seconds = time.value("seconds").toDouble();
    auto milliseconds = time.value("milliseconds").toDouble();
    return hours * 3600 * 1000 + minutes * 60 * 1000 + seconds * 1000 + milliseconds;
}

void setTime(QJsonValue time, Player& player)
{
    if(time.isObject())
        player.setTime(getTime(time.toObject()));
}

void setTotalTime(QJsonValue totalTime, Player& player)
{
    if(totalTime.isObject())
        player.setTotalTime(getTime(totalTime.toObject()));
}

void setBoolValue_(QJsonValue value, Player& player, void(Player::*fn)(bool), bool oldValue)
{
	if(value.isBool())
	{
		auto newValue = value.toBool();
		if(newValue != oldValue)
			(player.*fn)(newValue);
	}
}

void setIntValue_(QJsonValue value, Player& player, void(Player::*fn)(int), int oldValue)
{
	if(value.isDouble())
	{
		int newValue = static_cast<int>(value.toDouble());
		if(newValue != oldValue)
			(player.*fn)(newValue);
	}
}

void setDoubleValue(QJsonValue value, Player& player, void(Player::*fn)(double))
{
    if(value.isDouble())
        (player.*fn)(value.toDouble());
}

}

void PlayerService::updatePlayPause_()
{
    QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        QJsonRpcMessage message = reply->response();
        if(message.errorCode() == 0)
        {
            QJsonValue val = message.result();
            if(val.isObject())
            {
                QJsonObject obj = val.toObject();
                QJsonValue typeVal = obj.value("speed");
                if(typeVal.isDouble())
                {
                    int playerId = reply->request().params().toObject().value("playerid").toInt();
                    for(int i = 0; i < currentPlayers_.size(); ++i)
                    {
                        if(playerId == currentPlayers_[i]->playerId())
                        {
                            currentPlayers_[i]->setSpeed((int)typeVal.toDouble());
                        }
                    }
                }
            }
        }
        emit playersChanged();
    }
}

void PlayerService::refreshPlayerStatus(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    QJsonArray properties;
    properties.append(QString("speed"));
    properties.append(QString("position"));
    properties.append(QString("type"));
    properties.append(QString("percentage"));
    properties.append(QString("time"));
    properties.append(QString("totaltime"));
    properties.append(QString("playlistid"));
    properties.append(QString("canseek"));
    properties.append(QString("canshuffle"));
    properties.append(QString("canrepeat"));
    properties.append(QString("live"));
    properties.append(QString("repeat"));
    properties.append(QString("shuffled"));

    properties.append(QString("canmove"));
    properties.append(QString("canzoom"));
    properties.append(QString("canrotate"));
    properties.append(QString("canchangespeed"));
    properties.append(QString("subtitles"));
    properties.append(QString("currentsubtitle"));
    properties.append(QString("subtitleenabled"));
    properties.append(QString("audiostreams"));
    properties.append(QString("currentaudiostream"));
    parameters["properties"] = properties;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GetProperties", parameters);
	auto reply = client_->send(message);
    if(reply)
        connect(reply, &QJsonRpcServiceReply::finished, this, [this, playerId]() { this->refreshPlayerInfo_(playerId); });
}

void PlayerService::refreshPlayerInfo_(int playerId)
{
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply)
	{
		Player* player = nullptr;
		for(int i = 0; i < this->currentPlayers_.size(); ++i)
		{
			if(currentPlayers_[i]->playerId() == playerId)
				player = currentPlayers_[i];
		}
		if(!player)
			return;
		QJsonRpcMessage message = reply->response();
		if(message.errorCode() == 0)
		{
			QJsonValue val = message.result();
			if(val.isObject())
			{
				QJsonObject obj = val.toObject();
				QJsonValue typeVal = obj.value("type");
				if(!typeVal.isString())
					return;
				QString type = typeVal.toString();
				if(type != player->type())
					player->setType(type);
				setDoubleValue(obj.value("percentage"), *player, &Player::setPercentage);
				setTime(obj.value("time"), *player);
				setTotalTime(obj.value("totaltime"), *player);
				setIntValue_(obj.value("speed"), *player, &Player::setSpeed, player->speed());
				setIntValue_(obj.value("position"), *player, &Player::setPlaylistPosition, player->playlistPosition());
				setIntValue_(obj.value("playlistid"), *player, &Player::setPlaylistId, player->playlistId());
				setBoolValue_(obj.value("canseek"), *player, &Player::setCanSeek, player->canSeek());
				setBoolValue_(obj.value("canshuffle"), *player, &Player::setCanShuffle, player->canShuffle());
				setBoolValue_(obj.value("canrepeat"), *player, &Player::setCanRepeat, player->canRepeat());
				setBoolValue_(obj.value("live"), *player, &Player::setLive, player->live());
				setIntValue_(obj.value("repeat"), *player, &Player::setRepeat, player->repeat());
				setBoolValue_(obj.value("shuffled"), *player, &Player::setShuffled, player->shuffled());
				setBoolValue_(obj.value("canmove"), *player, &Player::setCanMove, player->canMove());
				auto subtitles = obj.value("subtitles").toArray();
				if(!subtitles.isEmpty())
				{
					std::vector<Subtitle*> subs;
					auto nosub = new Subtitle();
					nosub->setIndex(-1);
					nosub->setLanguage("");
					nosub->setName(tr("No subtitle"));
					subs.push_back(nosub);
					for(auto sub_ : subtitles)
					{
						auto sub = sub_.toObject();
						auto index = sub.value("index").toInt();
						auto language = sub.value("language").toString();
						auto name = sub.value("name").toString();
						auto s = new Subtitle();
						s->setIndex(index);
						s->setLanguage(language);
						if(name.size() > 0)
							s->setName(name);
						else
							s->setName(language);
						subs.push_back(s);
					}
					auto currentSub = obj.value("currentsubtitle").toObject();
					auto enabled = obj.value("subtitleenabled").toBool();
					if(enabled)
					{
						if(!currentSub.isEmpty())
						{
							auto index = currentSub.value("index");
							player->setSubtitles(std::move(subs), index.toInt());
						}
					}
					else
						player->setSubtitles(std::move(subs), -1);
				}
				auto audiostreams = obj.value("audiostreams").toArray();
				if(!audiostreams.isEmpty())
				{
					std::vector<AudioStream*> streams;
					for(auto stream_ : audiostreams)
					{
						auto stream = stream_.toObject();
						auto index = stream.value("index").toInt();
						auto language = stream.value("language").toString();
						auto name = stream.value("name").toString();
						auto s = new AudioStream();
						s->setIndex(index);
						s->setLanguage(language);
						if(name.size() > 0)
							s->setName(name);
						else
							s->setName(language);
						streams.push_back(s);
					}
					auto currentStream = obj.value("currentaudiostream");
					auto index = currentStream.toObject().value("index").toInt();
					player->setAudioStreams(std::move(streams), index);
				}
				//setBoolValue(obj.value("canzoom"), *player, &Player::setCanZoom);
				//setBoolValue(obj.value("canrotate"), *player, &Player::setCanRotate);
				//setBoolValue(obj.value("canchangespeed"), *player, &Player::setCanChangeSpeed);
			}
		}
		emit playersChanged();
	}
}

void PlayerService::updateConnectionStatus(int newStatus)
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

}
}
}
