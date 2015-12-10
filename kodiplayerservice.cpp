#include "kodiplayerservice.h"
#include "kodiclient.h"
#include "kodisettingsmanager.h"

KodiPlayerService::KodiPlayerService(QObject *parent) : QObject(parent)
{
    refreshTimer_.setInterval(5000);
    refreshTimer_.setSingleShot(false);
    connect(&refreshTimer_, &QTimer::timeout, this, &KodiPlayerService::refreshPlayerInfo);
    connect(&KodiClient::current(), SIGNAL(connectionStatusChanged(int)), this, SLOT(updateConnectionStatus(int)));
    connect(&KodiClient::current(), &KodiClient::playerStopped, this, &KodiPlayerService::refreshPlayerInfo);
    connect(&KodiClient::current(), &KodiClient::playerSpeedChanged, this, &KodiPlayerService::updatePlayerSpeed);
    refreshPlayerInfo();
}

KodiPlayerService &KodiPlayerService::instance()
{
    static KodiPlayerService service;
    return service;
}

void KodiPlayerService::refreshPlayerInfo()
{
    if(!refreshPending_)
    {
        refreshPending_ = true;
        QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.getActivePlayers");
        QJsonRpcServiceReply* reply = KodiClient::current().send(message);
        if(reply)
            connect(reply, &QJsonRpcServiceReply::finished, this, &KodiPlayerService::refreshPlayerInfoCb_);
        else
            refreshPending_ = false;
    }
}

void KodiPlayerService::playPause(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.PlayPause", parameters);
    QJsonRpcServiceReply* reply = KodiClient::current().send(message);
    if(reply)
        connect(reply, &QJsonRpcServiceReply::finished, this, &KodiPlayerService::updatePlayPause_);
}

const QList<KodiPlayer *> &KodiPlayerService::players()
{
    return currentPlayers_;
}

int KodiPlayerService::getPlayerId(QString type)
{
    for(KodiPlayer* player : currentPlayers_)
    {
        if(player->type() == type)
            return player->playerId();
    }
    return -1;
}

KodiPlayer *KodiPlayerService::getPlayer(QString type)
{
    for(KodiPlayer* player : currentPlayers_)
    {
        if(player->type() == type)
            return player;
    }
    return nullptr;
}

void KodiPlayerService::refreshPlayerInfoCb_()
{
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
                            KodiPlayer* player = new KodiPlayer(this);
                            player->setPlayerId(val2.toObject()["playerid"].toInt());
                            player->setType(val2.toObject()["type"].toString());
                            if(player->type() == "audio")
                                refreshAudioPlayerStatus_(player->playerId());
                            else if(player->type() == "video")
                                refreshVideoPlayerStatus(player->playerId());
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

void KodiPlayerService::refreshAudioPlayerStatus_(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    QJsonArray properties;
    properties.append(QString("speed"));
    properties.append(QString("position"));
    properties.append(QString("type"));
    parameters["properties"] = properties;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GetProperties", parameters);
    auto reply = KodiClient::current().send(message);
    if(reply)
        connect(reply, SIGNAL(finished()), this, SLOT(refreshAudioPlayerInfo_()));
}

void KodiPlayerService::refreshAudioPlayerInfo_()
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
                QJsonValue typeVal = obj.value("type");
                if(!typeVal.isString())
                    return;
                QString type = typeVal.toString();
                KodiPlayer* player = nullptr;
                for(int i = 0; i < this->currentPlayers_.size(); ++i)
                {
                    if(currentPlayers_[i]->type() == type)
                        player = currentPlayers_[i];
                }
                if(!player)
                    return;
                QJsonValue percentage = obj.value("percentage");
                if(percentage.isDouble())
                {

                }
                QJsonValue speed = obj.value("speed");
                if(speed.isDouble())
                    player->setSpeed((int)speed.toDouble());
                QJsonValue position = obj.value("position");
                if(position.isDouble())
                    player->setPlaylistPosition((int)position.toDouble());
            }
        }
    }
    emit playersChanged();
}

void KodiPlayerService::updatePlayPause_()
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

void KodiPlayerService::refreshVideoPlayerStatus(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    QJsonArray properties;
    properties.append(QString("speed"));
    properties.append(QString("position"));
    properties.append(QString("type"));
    parameters["properties"] = properties;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GetProperties", parameters);
    auto reply = KodiClient::current().send(message);
    if(reply)
        connect(reply, &QJsonRpcServiceReply::finished, this, [this, playerId]() { this->refreshVideoPlayerInfo_(playerId); });
}

void KodiPlayerService::refreshVideoPlayerInfo_(int playerId)
{
    QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        KodiPlayer* player = nullptr;
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
                QJsonValue percentage = obj.value("percentage");
                if(percentage.isDouble())
                {
                }
                QJsonValue speed = obj.value("speed");
                if(speed.isDouble())
                    player->setSpeed((int)speed.toDouble());
                QJsonValue position = obj.value("position");
                if(position.isDouble())
                    player->setPlaylistPosition((int)position.toDouble());
            }
        }
    }
    emit playersChanged();
}

void KodiPlayerService::updateConnectionStatus(int newStatus)
{
    if(newStatus == 2)
    {
        refreshPlayerInfo();
        if(KodiSettingsManager::instance().useHttpInterface())
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

void KodiPlayerService::updatePlayerSpeed(int playerId, int speed)
{
  //  bool found = false;
    for(KodiPlayer* player : currentPlayers_)
    {
        if(player->playerId() == playerId)
        {
    //        found = true;
            player->setSpeed(speed);
        }
    }
    //if(!found)
        refreshPlayerInfo();
/*    else
        emit playersChanged(); */
}
