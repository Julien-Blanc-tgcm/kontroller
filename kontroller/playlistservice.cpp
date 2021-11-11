#include "playlistservice.h"

#include "client.h"
#include "playerservice.h"
#include "utils.h"

#include <QDebug>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlaylistService::PlaylistService(Client* client, Player* player, QObject* parent) :
    QObject(parent), client_{client}, player_{player}
{
	connect(client_, &Client::playlistCleared, this, &PlaylistService::handlePlaylistCleared_);
	connect(client_, &Client::playlistElementRemoved, this, &PlaylistService::handlePlaylistElementRemoved);
	connect(client_, &Client::playlistElementAdded, this, &PlaylistService::handlePlaylistElementAdded);
	//	connect(client_, &Client::playlistCurrentItemChanged, this, &PlaylistService::setCurrentlyPlayedItem_);
	connect(player_, &Player::playlistIdChanged, this, &PlaylistService::playlistIdChanged);
	connect(player_, &Player::typeChanged, this, &PlaylistService::playlistTypeChanged);
	connect(player_, &Player::playlistPositionChanged, this, &PlaylistService::playlistPositionChanged);
}

int PlaylistService::playlistId() const
{
	return player_->playlistId();
}

QVariantList PlaylistService::items()
{
	QVariantList l;
	for (auto const& item : currentItems())
	{
		l.push_back(QVariant::fromValue(item));
	}
	return l;
}

QString PlaylistService::playlistType() const
{
	return player_->type();
}

int PlaylistService::playlistPosition() const
{
	return player_->playlistPosition();
}

//void PlaylistService::setPlaylistPosition(int playlistPosition)
//{
//	playlistPosition_ = playlistPosition;
//	emit playlistPositionChanged();
//	player_->setPlaylistPosition(playlistPosition);
//}

const QVector<PlaylistItem> PlaylistService::currentItems() const
{
	return currentItems_;
}

void PlaylistService::switchToItem(int position)
{
	QJsonObject parameters;
	int playerId = player_->playerId();
	if (playerId < 0)
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
	if (!refreshing_ && playlistId() >= 0)
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
		properties.append(QString("tvshowid"));
		properties.append(QString("duration"));
		parameters["properties"] = properties;
		parameters["playlistid"] = playlistId();
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
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		QJsonRpcMessage response = reply->response();
		if (response.errorCode() == 0)
		{
			QJsonValue items_ = response.result().toObject().value("items");
			if (items_.isArray())
			{
				QJsonArray items = items_.toArray();
				for (QJsonValue val_ : items)
				{
					if (val_.isObject())
					{
						QJsonObject val = val_.toObject();
						PlaylistItem item;
						item.setAlbumId(val.value("albumid").toInt());
						auto artistIds = val.value("artistid");
						if(artistIds.isArray() && artistIds.toArray().size() > 0)
						{
							item.setArtistId(artistIds.toArray()[0].toInt());
						}
						QString type = val.value("type").toString();
						int id = val.value("id").toInt();
						if (type == "song")
							item.setSongId(id);
						else if (type == "movie")
							item.setMovieId(id);
						else if (type == "episode")
							item.setEpisodeId(id);
						else if (type == "musicvideo")
							item.setMusicvideoId(id);
						else if (type == "picture")
							item.setPictureId(id);
						item.setFile(val.value("file").toString());
						item.setLabel(val.value("label").toString());
						if (val.value("title").toString().size() > 0)
							item.setLabel(val.value("title").toString()); // set title instead if available
						item.setType(val.value("type").toString());
						item.setAlbum(val.value("album").toString());
						item.setArtist(val.value("displayartist").toString());
						item.setFanart(getImageUrl(client_, val.value("fanart").toString()).toString());
						item.setThumbnail(getImageUrl(client_, val.value("thumbnail").toString()).toString());
						item.setTvshow(val.value("showtitle").toString());
						item.setTvshowId(val.value("tvshowid").toInt(0));
						item.setDuration(val.value("duration").toInt(0));
						currentItems_.push_back(item);
					}
				}
			}
//			setPlaylistPosition(player_->playlistPosition());
		}
		else
		{
			qDebug() << response;
		}
	}
	emit itemsChanged();
	if (restartAfterRefreshing_)
	{
		restartAfterRefreshing_ = false;
		refreshPlaylist_();
	}
}

void PlaylistService::handlePlaylistCleared_(int playlist)
{
	if (playlist == playlistId())
	{
		currentItems_.clear();
		emit itemsChanged();
	}
}

void PlaylistService::handlePlaylistElementRemoved(int playlist, int position)
{
	if (playlist == playlistId())
	{
		if (position < currentItems_.size())
		{
//			if (position < playlistPosition_)
//				playlistPosition_ -= 1;
			currentItems_.removeAt(position);
			emit itemsChanged();
		}
	}
}

void PlaylistService::handlePlaylistElementAdded(int playlistId)
{
	if (playlistId == this->playlistId())
		refreshPlaylist_();
}

void PlaylistService::clearPlaylist()
{
	QJsonRpcMessage message;
	QJsonObject params;
	params.insert("playlistid", playlistId());
	message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
	client_->send(message);
}

void PlaylistService::removeElement(int index)
{
	QJsonRpcMessage message;
	QJsonObject params;
	params.insert("playlistid", playlistId());
	params.insert("position", index);
	message = QJsonRpcMessage::createRequest("Playlist.Remove", params);
	client_->send(message);
}

void PlaylistService::refresh()
{
	refreshPlaylist_();
}

//void PlaylistService::setCurrentlyPlayedItem_(int playerId, QString type, int id, QString file)
//{
//	if (player_->playerId() != playerId)
//		return; // not for us
//	bool found = false;
//	for (int i = 0; !found && i < currentItems_.size(); ++i)
//	{
//		auto item = currentItems_[i];
//		if (type == "song")
//		{
//			if (item.songId() == id)
//			{
//				setPlaylistPosition(i);
//				found = true;
//			}
//		}
//		else if (type == "movie")
//		{
//			if (item.movieId() == id)
//			{
//				found = true;
////				setPlaylistPosition(i);
//			}
//		}
//		else if (type == "episode")
//		{
//			if (item.episodeId() == id)
//			{
//				found = true;
//				setPlaylistPosition(i);
//			}
//		}
//		else if (type == "musicvideo")
//		{
//			if (item.musicvideoId() == id)
//			{
//				found = true;
////				setPlaylistPosition(i);
//			}
//		}
//		else if (type == "picture")
//		{
//			if (item.file() == file)
//			{
//				found = true;
////				setPlaylistPosition(i);
//			}
//		}
//	}
//	if (!found)
//	{
//		qDebug() << "Current item not found, refreshing the playlist";
//		//refreshPlaylist_();
//	}
//}

void PlaylistService::handleGotoReply_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply != nullptr)
	{
		auto response = reply->response();
		if (response.result().isString() && response.result().toString() == QString::fromUtf8("OK"))
		{
			if (pendingPosition_ != -1)
			{
				pendingPosition_ = -1;
			}
		}
	}
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
