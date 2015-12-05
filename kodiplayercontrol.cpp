#include "kodiclient.h"
#include "kodiplayercontrol.h"
#include "kodisettingsmanager.h"
#include "kodiplayerservice.h"

KodiPlayerControl::KodiPlayerControl(QObject *parent) :
    QObject(parent)
{
    // forward event
    connect(&KodiPlayerService::instance(), &KodiPlayerService::playersChanged, this, &KodiPlayerControl::updatePlayers_);
}

void KodiPlayerControl::refreshPlayerInfo()
{
    KodiPlayerService::instance().refreshPlayerInfo();
}

void KodiPlayerControl::playPause(int playerId)
{
    KodiPlayerService::instance().playPause(playerId);
}

void KodiPlayerControl::next(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    parameters["to"] = QLatin1String("next");
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GoTo", parameters);
    KodiClient::current().send(message);
}

void KodiPlayerControl::previous(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    parameters["to"] = QLatin1String("previous");
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GoTo", parameters);
    KodiClient::current().send(message);
}

void KodiPlayerControl::moveToFirst(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    parameters["to"] = 0;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GoTo", parameters);
    KodiClient::current().send(message);
}

void KodiPlayerControl::stop(int playerId)
{
    QJsonObject parameters;
    parameters["playerid"] = playerId;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.Stop", parameters);
    KodiClient::current().send(message);
}

void KodiPlayerControl::updatePlayers_()
{
    players_ = KodiPlayerService::instance().players();
    emit playersChanged();
}

namespace
{
int playersPropCount(QQmlListProperty<KodiPlayer>* list)
{
    return static_cast<QList<KodiPlayer*>*>(list->data)->count();
}

KodiPlayer* playersPropAt(QQmlListProperty<KodiPlayer>* list, int index)
{
    return static_cast<QList<KodiPlayer*>*>(list->data)->at(index);
}
}

QQmlListProperty<KodiPlayer> KodiPlayerControl::players()
{
    return QQmlListProperty<KodiPlayer>(this, &players_, &playersPropCount, &playersPropAt);
}
