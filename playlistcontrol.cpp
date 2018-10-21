#include "playlistcontrol.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

PlaylistControl::PlaylistControl(QObject *parent) : QObject(parent)
{
    PlaylistService& service = PlaylistService::instance();
    connect(&service, &PlaylistService::playlistIdChanged, this, [this](){emit playlistIdChanged();});
    connect(&service, &PlaylistService::playlistPositionChanged, this, &PlaylistControl::playlistPositionChanged);
    connect(&service, &PlaylistService::playlistTypeChanged, this, &PlaylistControl::playlistTypeChanged);
    connect(&service, &PlaylistService::itemsChanged, this, &PlaylistControl::itemsChanged);
}

QQmlListProperty<PlaylistItem> PlaylistControl::items()
{
    return PlaylistService::instance().items();
}

int PlaylistControl::playlistId() const
{
    return PlaylistService::instance().playlistId();
}

QString PlaylistControl::playlistType() const
{
    return PlaylistService::instance().playlistType();
}

int PlaylistControl::playlistPosition() const
{
    return PlaylistService::instance().playlistPosition();
}

void PlaylistControl::switchToItem(int position)
{
    PlaylistService::instance().switchToItem(position);
}

void PlaylistControl::clearPlaylist()
{
    PlaylistService::instance().clearPlaylist();
}

void PlaylistControl::removeElement(int index)
{
    PlaylistService::instance().removeElement(index);
}

void PlaylistControl::setPlaylistType(QString type)
{
	PlaylistService::instance().setPlaylistType(type);
}

void PlaylistControl::refresh()
{
	PlaylistService::instance().refresh();
	// TODO : implement
}

}
}
}
