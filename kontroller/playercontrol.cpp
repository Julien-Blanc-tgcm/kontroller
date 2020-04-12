#include "client.h"
#include "playercontrol.h"
#include "playerservice.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlayerControl::PlayerControl(QObject *parent) :
    QObject(parent)
{
    // forward event
}

void PlayerControl::refreshPlayerInfo()
{
	client_->playerService()->refreshPlayerInfo();
}

void PlayerControl::playPause(int playerId)
{
	client_->playerService()->playPause(playerId);
}

void PlayerControl::next(int playerId)
{
	QJsonObject parameters;
	parameters["playerid"] = playerId;
	parameters["to"] = QLatin1String("next");
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GoTo", parameters);
	client_->send(message);
}

void PlayerControl::previous(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    parameters["to"] = QLatin1String("previous");
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GoTo", parameters);
	client_->send(message);
}

void PlayerControl::moveToFirst(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    parameters["to"] = 0;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GoTo", parameters);
	client_->send(message);
}

void PlayerControl::stop(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.Stop", parameters);
	client_->send(message);
}

void PlayerControl::seekBackward(int playerId)
{
    QJsonObject parameters;
    parameters.insert("playerid", playerId);
    parameters.insert("value", QLatin1String("smallbackward"));
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.Seek", parameters);
	client_->send(message);
}

void PlayerControl::seekForward(int playerId)
{
    QJsonObject parameters;
    parameters.insert("playerid", playerId);
    parameters.insert("value", QLatin1String("smallforward"));
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.Seek", parameters);
	client_->send(message);
}

void PlayerControl::seekToPercentage(int playerId, int percentage)
{
    QJsonObject parameters;
    parameters.insert("playerid", playerId);
    parameters.insert("value", percentage);
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.Seek", parameters);
	client_->send(message);
}

void PlayerControl::setShuffle(int playerId, bool shuffle)
{
    QJsonObject parameters;
    parameters.insert("playerid", playerId);
    parameters.insert("shuffle", shuffle);
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.SetShuffle", parameters);
	auto reply = client_->send(message);
    connect(reply, &QJsonRpcServiceReply::finished, this,
            [this, playerId, shuffle]() {this->handleShuffleResult_(playerId, shuffle);});
}

void PlayerControl::switchSubtitle(int playerId, int subtitleIndex)
{
	if(client_ == nullptr)
	{
		qDebug() << "Missing client for player control, check qml file";
		return;
	}
    QJsonObject parameters;
    parameters.insert("playerid", playerId);
    if(subtitleIndex == -1)
    {
        parameters.insert("enable", false);
        parameters.insert("subtitle", QString("off"));
    }
    else
    {
        parameters.insert("enable", true);
        parameters.insert("subtitle", subtitleIndex);
    }
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.SetSubtitle", parameters);
	auto reply = client_->send(message);
    connect(reply, &QJsonRpcServiceReply::finished, this,
            [this, playerId, subtitleIndex]() {
		for(auto player : client_->playerService()->players())
        {
            if(player->playerId() == playerId)
            {
                player->setCurrentSubtitleIndex(subtitleIndex);
            }
        }
    });
}

void PlayerControl::switchAudioStream(int playerId, int audioStreamIndex)
{
	if(client_ == nullptr)
	{
		qDebug() << "Missing client for PlayerControl, check qml file";
		return;
	}
    QJsonObject parameters;
    parameters.insert("playerid", playerId);
    parameters.insert("stream", audioStreamIndex);
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.SetAudioStream", parameters);
	auto reply = client_->send(message);
    connect(reply, &QJsonRpcServiceReply::finished, this, [this, playerId, audioStreamIndex]() {
		for(auto player : client_->playerService()->players())
        {
            if(player->playerId() == playerId)
                player->setCurrentAudioStreamIndex(audioStreamIndex);
        }
    });
}

void PlayerControl::setClient(Client* client)
{
	if (client_ == client)
		return;

	if(client_ != nullptr)
		disconnect(client_->playerService(), &PlayerService::playersChanged, this, &PlayerControl::updatePlayers_);

	client_ = client;
	connect(client_->playerService(), &PlayerService::playersChanged, this, &PlayerControl::updatePlayers_);
	emit clientChanged(client_);
	updatePlayers_(); // update when client is set
}

void PlayerControl::updatePlayers_()
{
	players_.clear();
	for(Player* player : client_->playerService()->players())
		players_.push_back(player);
	emit playersChanged();
}

void PlayerControl::handleShuffleResult_(int playerId, bool shuffle)
{
    auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
		for(auto player : client_->playerService()->players())
        {
            if(player->playerId() == playerId)
                player->setShuffled(shuffle);
        }
    }
}

namespace
{
int playersPropCount(QQmlListProperty<Player>* list)
{
    return static_cast<QList<Player*>*>(list->data)->count();
}

Player* playersPropAt(QQmlListProperty<Player>* list, int index)
{
    auto l = static_cast<QList<Player*>*>(list->data);
    if(index < l->size())
        return (*l)[index];
    return nullptr;
}
}

QQmlListProperty<Player> PlayerControl::players()
{
    return QQmlListProperty<Player>(this, &players_, &playersPropCount, &playersPropAt);
}

Client* PlayerControl::client() const
{
	return client_;
}

}
}
}
