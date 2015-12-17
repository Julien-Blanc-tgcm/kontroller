#include "playlistitem.h"

QString PlaylistItem::type() const
{
    return type_;
}

void PlaylistItem::setType(const QString &type)
{
    type_ = type;
    emit typeChanged();
}

QString PlaylistItem::file() const
{
    return file_;
}

void PlaylistItem::setFile(const QString &file)
{
    file_ = file;
    emit fileChanged();
}

QString PlaylistItem::label() const
{
    return label_;
}

void PlaylistItem::setLabel(const QString &label)
{
    label_ = label;
    emit labelChanged();
}

int PlaylistItem::artistId() const
{
    return artistId_;
}

void PlaylistItem::setArtistId(int artistId)
{
    artistId_ = artistId;
    emit artistIdChanged();
}

int PlaylistItem::albumId() const
{
    return albumId_;
}

void PlaylistItem::setAlbumId(int albumId)
{
    albumId_ = albumId;
    emit albumIdChanged();
}

PlaylistItem::PlaylistItem(QObject *parent) : QObject(parent)
{

}

