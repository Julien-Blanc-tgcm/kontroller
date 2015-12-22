#include "playerservice.h"
#include "client.h"
#include "settingsmanager.h"
#include "playlistservice.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlayerService::PlayerService(QObject *parent) : QObject(parent)
{
    refreshTimer_.setInterval(5000);
    refreshTimer_.setSingleShot(false);
    connect(&refreshTimer_, &QTimer::timeout, this, &PlayerService::refreshPlayerInfo);
    connect(&Client::current(), SIGNAL(connectionStatusChanged(int)), this, SLOT(updateConnectionStatus(int)));
    connect(&Client::current(), &Client::playerStopped, this, &PlayerService::stopPlayer_);
    connect(&Client::current(), &Client::playerSpeedChanged, this, &PlayerService::updatePlayerSpeed);
    connect(&Client::current(), &Client::playerSeekChanged, this, &PlayerService::updatePlayerSeek_);
    connect(&playerRefreshTimer_, &QTimer::timeout, this, &PlayerService::refreshPlayerInfo);
    playerRefreshTimer_.setInterval(200);
    playerRefreshTimer_.setSingleShot(true);
    refreshPlayerInfo();
}

PlayerService &PlayerService::instance()
{
    static PlayerService service;
    return service;
}

void PlayerService::refreshPlayerInfo()
{
    if(!refreshPending_)
    {
        refreshPending_ = true;
        QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.getActivePlayers");
        QJsonRpcServiceReply* reply = Client::current().send(message);
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
    QJsonRpcServiceReply* reply = Client::current().send(message);
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
    for(auto player: currentPlayers_)
        player->deleteLater();
    currentPlayers_.clear();
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
                            Player* player = new Player(this);
                            player->setPlayerId(val2.toObject()["playerid"].toInt());
                            player->setType(val2.toObject()["type"].toString());
                            refreshPlayerStatus(player->playerId());
                            currentPlayers_.push_back(player);
                        }
                    }
                }
            }
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

void setBoolValue(QJsonValue value, Player& player, void(Player::*fn)(bool))
{
    if(value.isBool())
        (player.*fn)(value.toBool());
}

void setIntValue(QJsonValue value, Player& player, void(Player::*fn)(int))
{
    if(value.isDouble())
        (player.*fn)((int)value.toDouble());
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
    parameters["properties"] = properties;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GetProperties", parameters);
    auto reply = Client::current().send(message);
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
                player->setType(type);
                setDoubleValue(obj.value("percentage"), *player, &Player::setPercentage);
                setTime(obj.value("time"), *player);
                setTotalTime(obj.value("totaltime"), *player);
                setIntValue(obj.value("speed"), *player, &Player::setSpeed);
                setIntValue(obj.value("position"), *player, &Player::setPlaylistPosition);
                setIntValue(obj.value("playlistid"), *player, &Player::setPlaylistId);
                setBoolValue(obj.value("canseek"), *player, &Player::setCanSeek);
                setBoolValue(obj.value("canshuffle"), *player, &Player::setCanShuffle);
                setBoolValue(obj.value("canrepeat"), *player, &Player::setCanRepeat);
                setBoolValue(obj.value("live"), *player, &Player::setLive);
                setIntValue(obj.value("repeat"), *player, &Player::setRepeat);
                setBoolValue(obj.value("shuffled"), *player, &Player::setShuffled);
                setBoolValue(obj.value("canmove"), *player, &Player::setCanMove);
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
                //setBoolValue(obj.value("canzoom"), *player, &Player::setCanZoom);
                //setBoolValue(obj.value("canrotate"), *player, &Player::setCanRotate);
                //setBoolValue(obj.value("canchangespeed"), *player, &Player::setCanChangeSpeed);
            }
        }
    }
    emit playersChanged();
}

void PlayerService::updateConnectionStatus(int newStatus)
{
    if(newStatus == 2)
    {
        refreshPlayerInfo();
        if(SettingsManager::instance().useHttpInterface())
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
