#include "playinginformation.h"

#include "client.h"
#include "player.h"
#include "playerservice.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlayingInformation::PlayingInformation(Player* player, QObject *parent) : QObject(parent),
    player_{player}
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

PlaylistItem PlayingInformation::currentItem()
{
	int position = player_->playlistPosition();
	if(position >= 0 && player_->playlistService()->currentItems().size() > position)
		return player_->playlistService()->currentItems().at(position);
	return item_;
}

PlaylistItem PlayingInformation::nextItem()
{
	int position = player_->playlistPosition() + 1;
	if(position >= 0 && player_->playlistService()->currentItems().size() > position)
		return player_->playlistService()->currentItems().at(position);
	return PlaylistItem();
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

void PlayingInformation::setItem(PlaylistItem item)
{
	item_ = item;
	emit currentItemChanged();
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
