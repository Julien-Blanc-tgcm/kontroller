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

Client* PlaylistService::client() const
{
	return client_;
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
	for(auto player : client_->playerService()->players())
    {
        if(player->type() == playlistType_)
        {
            player->setPlaylistPosition(playlistPosition_);
        }
    }
	client_->playerService()->refreshPlayerInfo();
}

const QList<PlaylistItem*> PlaylistService::currentItems() const
{
    return currentItems_;
}

void PlaylistService::switchToItem(int position)
{
	QJsonObject parameters;
	int playerId = client_->playerService()->getPlayerId(playlistType_);
	if(playerId < 0)
		return;
	parameters["playerid"] = playerId;
	parameters["to"] = position;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Player.GoTo", parameters);
	pendingPosition_ = position;
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &PlaylistService::handleGotoReply_);
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
		auto reply = client_->send(message);
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
						item->setFanart(getImageUrl(client_, val.value("fanart").toString()).toString());
						item->setThumbnail(getImageUrl(client_, val.value("thumbnail").toString()).toString());
                        item->setTvshow(val.value("showtitle").toString());
                        currentItems_.push_back(item);
                    }
                }
            }
			Player* player = client_->playerService()->getPlayer(playlistType_);
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
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &PlaylistService::findMatchingPlaylistCb_);
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
	client_->send(message);
}

void PlaylistService::removeElement(int index)
{
	QJsonRpcMessage message;
	QJsonObject params;
	params.insert("playlistid", playlistId_);
	params.insert("position", index);
	message = QJsonRpcMessage::createRequest("Playlist.Remove", params);
	client_->send(message);
}

void PlaylistService::refresh()
{
	refreshPlaylist_();
}

void PlaylistService::setClient(Client* client)
{
	if (client_ == client)
		return;
	if(client_ != nullptr)
	{
		disconnect(client_, &Client::playlistCleared, this, &PlaylistService::handlePlaylistCleared_);
		disconnect(client_, &Client::playlistElementRemoved, this, &PlaylistService::handlePlaylistElementRemoved);
		disconnect(client_, &Client::playlistElementAdded, this, &PlaylistService::handlePlaylistElementAdded);
		disconnect(client_, &Client::playlistCurrentItemChanged, this, &PlaylistService::setCurrentlyPlayedItem_);
		disconnect(client_->playerService(), &PlayerService::playersChanged, this, &PlaylistService::handlePlayerChanged_);
	}
	client_ = client;
	connect(client_, &Client::playlistCleared, this, &PlaylistService::handlePlaylistCleared_);
	connect(client_, &Client::playlistElementRemoved, this, &PlaylistService::handlePlaylistElementRemoved);
	connect(client_, &Client::playlistElementAdded, this, &PlaylistService::handlePlaylistElementAdded);
	connect(client_, &Client::playlistCurrentItemChanged, this, &PlaylistService::setCurrentlyPlayedItem_);
	connect(client_->playerService(), &PlayerService::playersChanged, this, &PlaylistService::handlePlayerChanged_);
	emit clientChanged(client_);
	handlePlayerChanged_();
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
	if(client_->playerService()->players().size() > 0)
	{
		setPlaylistType(client_->playerService()->players().at(0)->type());
	}
	else
	{
		playlistType_.clear();
	}
}

void PlaylistService::handleGotoReply_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply != nullptr)
	{
		auto response = reply->response();
		if(response.result().isString() && response.result().toString() == QString::fromUtf8("OK"))
		{
			if(pendingPosition_ != -1)
			{
				setPlaylistPosition(pendingPosition_);
				pendingPosition_ = -1;
			}
		}
	}
}


}
}
}
