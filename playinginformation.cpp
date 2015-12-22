#include "playinginformation.h"
#include "player.h"
#include "playerservice.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlayingInformation::PlayingInformation(QObject *parent) : QObject(parent)
{
    connect(&PlayerService::instance(), &PlayerService::playersChanged, this, &PlayingInformation::refreshCurrentPlayer_);
    connect(&PlaylistService::instance(), &PlaylistService::itemsChanged, this, &PlayingInformation::refreshCurrentPlayer_);
    connect(&PlaylistService::instance(), &PlaylistService::playlistPositionChanged, this, &PlayingInformation::refreshCurrentPlayer_);
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
    Player* player = nullptr;
    for(auto p : PlayerService::instance().players())
    {
        player = p;
    }
    if(player == nullptr)
        return nullptr;
    int position = player->playlistPosition();
    if(position >= 0 && PlaylistService::instance().currentItems().size() > position)
        return PlaylistService::instance().currentItems().at(position);
    else
        return nullptr;
}

PlaylistItem* PlayingInformation::nextItem()
{
    Player* player = nullptr;
    for(auto p : PlayerService::instance().players())
    {
        player = p;
    }
    if(player == nullptr)
        return nullptr;
    int position = player->playlistPosition() + 1;
    if(position >= 0 && PlaylistService::instance().currentItems().size() > position)
        return PlaylistService::instance().currentItems().at(position);
    else
        return nullptr;
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

void PlayingInformation::refreshCurrentPlayer_()
{
    Player* player = nullptr;
    for(auto p : PlayerService::instance().players())
    {
        player = p;
    }
    if(player != nullptr)
    {
        setPlayerType(player->type());
        int position = player->playlistPosition();
        auto list = PlaylistService::instance().currentItems();
        if(list.size() > position && position >= 0)
        {
            auto item = list.at(position);
            setMediaId(item->file());
            setMediaTitle(item->label());
            setArtistId(item->artistId());
//            setAlbumId(item->albumId());
        }
        connect(player, &Player::playlistPositionChanged, this, &PlayingInformation::currentItemChanged);
    }
    emit currentItemChanged();
}

}
}
}
