#include "playinginformation.h"

#include "player.h"

#include "client.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlayingInformation::PlayingInformation(QObject *parent) : QObject(parent)
{
}

QString PlayingInformation::playerType() const
{
	return playerType_;
}

QString PlayingInformation::mediaTitle() const
{
	return mediaTitle_;
}

QString PlayingInformation::mediaId() const
{
	return mediaId_;
}

int PlayingInformation::artistId() const
{
	return artistId_;
}

PlaylistItem* PlayingInformation::currentItem()
{
	if(client_ == nullptr)
		return nullptr;
	Player* player = nullptr;
	for(auto p : client_->playerService()->players())
	{
		player = p;
	}
	if(player == nullptr)
		return nullptr;
	int position = player->playlistPosition();
	if(position >= 0 && playlistService_->currentItems().size() > position)
		return playlistService_->currentItems().at(position);
	else
		return item_;
}

PlaylistItem* PlayingInformation::nextItem()
{
	if(client_ == nullptr)
		return nullptr;
	Player* player = nullptr;
	for(auto p : client_->playerService()->players())
	{
		player = p;
	}
	if(player == nullptr)
		return nullptr;
	int position = player->playlistPosition() + 1;
	if(position >= 0 && playlistService_->currentItems().size() > position)
		return playlistService_->currentItems().at(position);
	else
		return nullptr;
}

PlaylistService* PlayingInformation::playlistService() const
{
	return playlistService_;
}

Client* PlayingInformation::client() const
{
	return client_;
}

void PlayingInformation::refreshCurrentlyPlaying_(int playerid)
{
	QJsonObject parameters;
	parameters["playerid"] = playerid;
	QJsonArray properties;
	properties.append("thumbnail");
	properties.append("fanart");
	properties.append("title");
	properties.append("file");
	properties.append("artistid");
	properties.append("albumid");
	properties.append("artist");
	properties.append("album");
	properties.append("tvshowid");
	parameters["properties"] = properties;
	auto mess = QJsonRpcMessage::createRequest("Player.getItem", parameters);
	auto reply = client_->send(mess);
	connect(reply, &QJsonRpcServiceReply::finished, this, &PlayingInformation::handleGetItemResponse_);
}

void PlayingInformation::setPlayerType(QString playerType)
{
	if (playerType_ == playerType)
		return;

	playerType_ = playerType;
	emit playerTypeChanged(playerType);
}

void PlayingInformation::setMediaTitle(QString mediaTitle)
{
	if (mediaTitle_ == mediaTitle)
		return;

	mediaTitle_ = mediaTitle;
	emit MediaTitleChanged(mediaTitle);
}

void PlayingInformation::setMediaId(QString mediaId)
{
	if (mediaId_ == mediaId)
		return;

	mediaId_ = mediaId;
	emit mediaIdChanged(mediaId);
}

void PlayingInformation::setArtistId(int artistId)
{
	if (artistId_ == artistId)
		return;

	artistId_ = artistId;
	emit artistIdChanged(artistId);
}

void PlayingInformation::setClient(Client* client)
{
	if (client_ == client)
		return;

	if(client_ != nullptr)
	{
		disconnect(client_->playerService(), &PlayerService::playersChanged, this, &PlayingInformation::refreshCurrentPlayer_);
	}
	if(playlistService_ != nullptr)
	{
		disconnect(playlistService_, &PlaylistService::itemsChanged, this, &PlayingInformation::refreshCurrentPlayer_);
		disconnect(playlistService_, &PlaylistService::playlistPositionChanged, this,
		        &PlayingInformation::refreshCurrentPlayer_);
		playlistService_->deleteLater();
		playlistService_ = nullptr;
	}

	client_ = client;
	playlistService_ = new PlaylistService{this};
	playlistService_->setClient(client_);
	connect(client_->playerService(), &PlayerService::playersChanged, this, &PlayingInformation::refreshCurrentPlayer_);
	connect(playlistService_, &PlaylistService::itemsChanged, this, &PlayingInformation::refreshCurrentPlayer_);
	connect(playlistService_, &PlaylistService::playlistPositionChanged, this,
	        &PlayingInformation::refreshCurrentPlayer_);

	emit clientChanged(client_);
}

void PlayingInformation::refreshCurrentPlayer_()
{
	Player* player = nullptr;
	for(auto p : client_->playerService()->players())
	{
		player = p;
	}
	if(player != nullptr)
	{
		setPlayerType(player->type());
		int position = player->playlistPosition();
		auto list = playlistService_->currentItems();
		if(list.size() > position && position >= 0)
		{
			auto item = list.at(position);
			setMediaId(item->file());
			setMediaTitle(item->label());
			setArtistId(item->artistId());
			//            setAlbumId(item->albumId());
		}
		else
		{
			// no playlist information. Use direct item information
			refreshCurrentlyPlaying_(player->playerId());
		}
		connect(player, &Player::playlistPositionChanged, this, &PlayingInformation::currentItemChanged);
	}
	emit currentItemChanged();
}

void PlayingInformation::handleGetItemResponse_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply != nullptr)
	{
		if(item_ != nullptr)
			item_->deleteLater();
		item_ = new PlaylistItem(this);
		auto resp = reply->response().toObject().value("result").toObject();
		auto itemTmp = resp.value("item");
		if(itemTmp.isObject())
		{
			auto item = itemTmp.toObject();
			item_->setFile(item.value("file").toString());
			item_->setType(item.value("type").toString());
			item_->setThumbnail(getImageUrl(client_, item.value("thumbnail").toString()).toString());
			item_->setFanart(getImageUrl(client_, item.value("fanart").toString()).toString());
			item_->setLabel(item.value("title").toString());
		}
	}
}

}
}
}
