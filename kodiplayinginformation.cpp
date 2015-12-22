#include "kodiplayinginformation.h"
#include "kodiplayer.h"
#include "kodiplayerservice.h"

KodiPlayingInformation::KodiPlayingInformation(QObject *parent) : QObject(parent)
{
    connect(&KodiPlayerService::instance(), &KodiPlayerService::playersChanged, this, &KodiPlayingInformation::refreshCurrentPlayer_);
    connect(&PlaylistService::instance(), &PlaylistService::itemsChanged, this, &KodiPlayingInformation::refreshCurrentPlayer_);
    connect(&PlaylistService::instance(), &PlaylistService::playlistPositionChanged, this, &KodiPlayingInformation::refreshCurrentPlayer_);
}

QString KodiPlayingInformation::playerType() const
{
    return playerType_;
}

QString KodiPlayingInformation::mediaTitle() const
{
    return mediaTitle_;
}

QString KodiPlayingInformation::mediaId() const
{
    return mediaId_;
}

int KodiPlayingInformation::artistId() const
{
    return artistId_;
}

PlaylistItem* KodiPlayingInformation::currentItem()
{
    KodiPlayer* player = nullptr;
    for(auto p : KodiPlayerService::instance().players())
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

PlaylistItem* KodiPlayingInformation::nextItem()
{
    KodiPlayer* player = nullptr;
    for(auto p : KodiPlayerService::instance().players())
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

void KodiPlayingInformation::setPlayerType(QString playerType)
{
    if (playerType_ == playerType)
        return;

    playerType_ = playerType;
    emit playerTypeChanged(playerType);
}

void KodiPlayingInformation::setMediaTitle(QString mediaTitle)
{
    if (mediaTitle_ == mediaTitle)
        return;

    mediaTitle_ = mediaTitle;
    emit MediaTitleChanged(mediaTitle);
}

void KodiPlayingInformation::setMediaId(QString mediaId)
{
    if (mediaId_ == mediaId)
        return;

    mediaId_ = mediaId;
    emit mediaIdChanged(mediaId);
}

void KodiPlayingInformation::setArtistId(int artistId)
{
    if (artistId_ == artistId)
        return;

    artistId_ = artistId;
    emit artistIdChanged(artistId);
}

void KodiPlayingInformation::refreshCurrentPlayer_()
{
    KodiPlayer* player = nullptr;
    for(auto p : KodiPlayerService::instance().players())
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
        connect(player, &KodiPlayer::playlistPositionChanged, this, &KodiPlayingInformation::currentItemChanged);
    }
    emit currentItemChanged();
}

