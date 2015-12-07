#include "playlistservice.h"
#include "kodiclient.h"
#include "kodiplayerservice.h"

PlaylistService::PlaylistService(QObject *parent) : QObject(parent),
    refreshing_(false),
    restartAfterRefreshing_(false)
{
    connect(&KodiPlayerService::instance(), &KodiPlayerService::playersChanged, this, &PlaylistService::refreshPlaylist_);
    connect(&KodiClient::current(), &KodiClient::playlistCleared, this, &PlaylistService::handlePlaylistCleared_);
    connect(&KodiClient::current(), &KodiClient::playlistElementRemoved, this, &PlaylistService::handlePlaylistElementRemoved);
}

int PlaylistService::playlistId() const
{
    return playlistId_;
}

void PlaylistService::setPlaylistId(int playlistId)
{
    playlistId_ = playlistId;
    emit playlistIdChanged();
    refreshPlaylist_();
}

namespace
{

int itemsPropCount(QQmlListProperty<KodiPlaylistItem>* list)
{
    return static_cast<QList<KodiPlaylistItem*>*>(list->data)->count();
}

KodiPlaylistItem* itemsPropAt(QQmlListProperty<KodiPlaylistItem>* list, int index)
{
    return static_cast<QList<KodiPlaylistItem*>*>(list->data)->at(index);
}

}

QQmlListProperty<KodiPlaylistItem> PlaylistService::items()
{
    return QQmlListProperty<KodiPlaylistItem>(this, &currentItems_, &itemsPropCount, &itemsPropAt);
}

QString PlaylistService::playlistType() const
{
    return playlistType_;
}

void PlaylistService::setPlaylistType(const QString &playlistType)
{
    playlistType_ = playlistType;
    playlistId_ = -1;
    findMatchingPlaylist_();
    emit playlistTypeChanged();
}

int PlaylistService::playlistPosition() const
{
    return playlistPosition_;
}

void PlaylistService::setPlaylistPosition(int playlistPosition)
{
    playlistPosition_ = playlistPosition;
    emit playlistPositionChanged();
    emit itemsChanged();
}

void PlaylistService::switchToItem(int position)
{
    QJsonObject parameters;
    int playerId = KodiPlayerService::instance().getPlayerId(playlistType_);
    if(playerId < 0)
        return;
    parameters["playerid"] = playerId;
    parameters["to"] = position;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GoTo", parameters);
    KodiClient::current().send(message);
}

void PlaylistService::refreshPlaylist_()
{
    if(!refreshing_)
    {
        QJsonObject parameters;
        QJsonArray properties;
        properties.append(QString("title"));
        properties.append(QString("artist"));
        properties.append(QString("track"));
        properties.append(QString("artistid"));
        properties.append(QString("albumid"));
        properties.append(QString("file"));
        properties.append(QString("genre"));
        parameters["properties"] = properties;
        parameters["playlistid"] = playlistId_;
        QJsonRpcMessage message = QJsonRpcMessage::createRequest("Playlist.GetItems", parameters);
        auto reply = KodiClient::current().send(message);
        connect(reply, SIGNAL(finished()), this, SLOT(refreshPlaylistCb_()));
    }
    else
        restartAfterRefreshing_ = true;
}

void PlaylistService::refreshPlaylistCb_()
{
    refreshing_ = false;
    currentItems_.clear();
    QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        QJsonRpcMessage response = reply->response();
        if(response.errorCode() == 0)
        {
            QJsonValue items_ = response.result().toObject().value("items");
            if(items_.isArray())
            {
                QJsonArray items = items_.toArray();
                for(QJsonValue val_ : items)
                {
                    if(val_.isObject())
                    {
                        QJsonObject val = val_.toObject();
                        KodiPlaylistItem* item = new KodiPlaylistItem(this);
                        item->setAlbumId(val.value("albumid").toInt());
                        item->setArtistId(val.value("artistid").toInt());
                        item->setFile(val.value("file").toString());
                        item->setLabel(val.value("label").toString());
                        item->setType(val.value("type").toString());
                        currentItems_.push_back(item);
                    }
                }
            }
            KodiPlayer* player = KodiPlayerService::instance().getPlayer(playlistType_);
            if(player)
            {
                setPlaylistPosition(player->playlistPosition());
            }
        }
        else
            qDebug() << response;
    }
    emit itemsChanged();
    if(restartAfterRefreshing_)
    {
        restartAfterRefreshing_ = false;
        refreshPlaylist_();
    }
}

void PlaylistService::findMatchingPlaylist_()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Playlist.GetPlaylists");
    auto reply = KodiClient::current().send(message);
    connect(reply, SIGNAL(finished()), this, SLOT(findMatchingPlaylistCb_()));
}

void PlaylistService::findMatchingPlaylistCb_()
{
    QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        QJsonRpcMessage response = reply->response();
        if(response.errorCode() == 0)
        {
            QJsonValue items_ = response.result().toArray();
            if(items_.isArray())
            {
                QJsonArray items = items_.toArray();
                for(QJsonValue val_ : items)
                {
                    if(val_.isObject())
                    {
                        QJsonObject val = val_.toObject();
                        QString type = val.value("type").toString();
                        if(type == playlistType_)
                        {
                            QJsonValue pid = val.value("playlistid");
                            if(pid.isDouble())
                            {
                                setPlaylistId((int)pid.toDouble());
                            }
                        }
                    }
                }
            }
        }
    }
}

void PlaylistService::handlePlaylistCleared_(int playlist)
{
    if(playlist == playlistId_)
    {
        currentItems_.clear();
        emit itemsChanged();
    }
}

void PlaylistService::handlePlaylistElementRemoved(int playlist, int position)
{
    if(playlist == playlistId_)
    {
        if(position < currentItems_.size())
        {
            if(position < playlistPosition_)
                playlistPosition_ -= 1;
            currentItems_.removeAt(position);
            emit itemsChanged();
        }
    }
}

void PlaylistService::handlePlaylistElementAdded(int playlistId)
{
    if(playlistId == playlistId_)
        refreshPlaylist_();
}

void PlaylistService::clearPlaylist()
{
    QJsonRpcMessage message;
    QJsonObject params;
    params.insert("playlistid", playlistId_);
    message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
    KodiClient::current().send(message);
}

void PlaylistService::removeElement(int index)
{
    QJsonRpcMessage message;
    QJsonObject params;
    params.insert("playlistid", playlistId_);
    params.insert("position", index);
    message = QJsonRpcMessage::createRequest("Playlist.Remove", params);
    KodiClient::current().send(message);
}
