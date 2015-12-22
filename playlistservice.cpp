#include "playlistservice.h"
#include "client.h"
#include "playerservice.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlaylistService::PlaylistService(QObject *parent) : QObject(parent),
    playlistId_(-1),
    refreshing_(false),
    restartAfterRefreshing_(false)
{
    connect(&PlayerService::instance(), &PlayerService::playersChanged, this, &PlaylistService::handlePlayerChanged_);
    connect(&Client::current(), &Client::playlistCleared, this, &PlaylistService::handlePlaylistCleared_);
    connect(&Client::current(), &Client::playlistElementRemoved, this, &PlaylistService::handlePlaylistElementRemoved);
    connect(&Client::current(), &Client::playlistCurrentItemChanged, this, &PlaylistService::setCurrentlyPlayedItem_);
}

PlaylistService&PlaylistService::instance()
{
    static PlaylistService inst;
    return inst;
}

int PlaylistService::playlistId() const
{
    return playlistId_;
}

void PlaylistService::setPlaylistId(int playlistId)
{
    if(playlistId != playlistId_)
    {
        playlistId_ = playlistId;
        emit playlistIdChanged();
        refreshPlaylist_();
    }
}

namespace
{

int itemsPropCount(QQmlListProperty<PlaylistItem>* list)
{
    return static_cast<QList<PlaylistItem*>*>(list->data)->count();
}

PlaylistItem* itemsPropAt(QQmlListProperty<PlaylistItem>* list, int index)
{
    auto l = static_cast<QList<PlaylistItem*>*>(list->data);
    if(index < l->size())
        return (*l)[index];
    return nullptr;
}

}

QQmlListProperty<PlaylistItem> PlaylistService::items()
{
    return QQmlListProperty<PlaylistItem>(this, &currentItems_, &itemsPropCount, &itemsPropAt);
}

QString PlaylistService::playlistType() const
{
    return playlistType_;
}

void PlaylistService::setPlaylistType(const QString &playlistType)
{
    if(playlistType != playlistType_)
    {
        playlistType_ = playlistType;
        //    playlistId_ = -1;
        findMatchingPlaylist_();
        emit playlistTypeChanged();
    }
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
    for(auto player : PlayerService::instance().players())
    {
        if(player->type() == playlistType_)
        {
            player->setPlaylistPosition(playlistPosition_);
        }
    }
    PlayerService::instance().refreshPlayerInfo();
}

const QList<PlaylistItem*> PlaylistService::currentItems() const
{
    return currentItems_;
}

void PlaylistService::switchToItem(int position)
{
    QJsonObject parameters;
    int playerId = PlayerService::instance().getPlayerId(playlistType_);
    if(playerId < 0)
        return;
    parameters["playerid"] = playerId;
    parameters["to"] = position;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GoTo", parameters);
    Client::current().send(message);
}

void PlaylistService::refreshPlaylist_()
{
    if(!refreshing_ && playlistId_ >= 0)
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
        properties.append(QString("album"));
        properties.append(QString("displayartist"));
        properties.append(QString("albumartist"));
        properties.append(QString("fanart"));
        properties.append(QString("thumbnail"));
        properties.append(QString("showtitle"));
        parameters["properties"] = properties;
        parameters["playlistid"] = playlistId_;
        QJsonRpcMessage message = QJsonRpcMessage::createRequest("Playlist.GetItems", parameters);
        auto reply = Client::current().send(message);
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
                        PlaylistItem* item = new PlaylistItem(this);
                        item->setAlbumId(val.value("albumid").toInt());
                        item->setArtistId(val.value("artistid").toInt());
                        QString type = val.value("type").toString();
                        int id = val.value("id").toInt();
                        if(type == "song")
                            item->setSongId(id);
                        else if(type == "movie")
                            item->setMovieId(id);
                        else if(type == "episode")
                            item->setEpisodeId(id);
                        else if(type == "musicvideo")
                            item->setMusicvideoId(id);
                        item->setFile(val.value("file").toString());
                        item->setLabel(val.value("label").toString());
                        item->setType(val.value("type").toString());
                        item->setAlbum(val.value("album").toString());
                        item->setArtist(val.value("displayartist").toString());
                        item->setFanart(getImageUrl(val.value("fanart").toString()).toString());
                        item->setThumbnail(getImageUrl(val.value("thumbnail").toString()).toString());
                        item->setTvshow(val.value("showtitle").toString());
                        currentItems_.push_back(item);
                    }
                }
            }
            Player* player = PlayerService::instance().getPlayer(playlistType_);
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
    auto reply = Client::current().send(message);
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
    Client::current().send(message);
}

void PlaylistService::removeElement(int index)
{
    QJsonRpcMessage message;
    QJsonObject params;
    params.insert("playlistid", playlistId_);
    params.insert("position", index);
    message = QJsonRpcMessage::createRequest("Playlist.Remove", params);
    Client::current().send(message);
}

void PlaylistService::setCurrentlyPlayedItem_(int /*playerId*/, QString type, int id)
{
    bool found = false;
    for(int i = 0; i < currentItems_.size(); ++i)
    {
        auto item = currentItems_[i];
        if(type == "song")
        {
            if(item->songId() == id)
            {
                setPlaylistPosition(i);
                found = true;
            }
        }
        else if(type == "movie")
        {
            if(item->movieId() == id)
            {
                found = true;
                setPlaylistPosition(i);
            }
        }
        else if(type == "episode")
        {
            if(item->episodeId() == id)
            {
                found = true;
                setPlaylistPosition(i);
            }
        }
        else if(type == "musicvideo")
            if(item->musicvideoId() == id)
            {
                found = true;
                setPlaylistPosition(i);
            }
    }
    if(!found)
        refreshPlaylist_();
}

void PlaylistService::handlePlayerChanged_()
{
    if(PlayerService::instance().players().size() > 0)
    {
        setPlaylistType(PlayerService::instance().players().at(0)->type());
    }
}


}
}
}
