#include "kodiplaylistitem.h"

QString KodiPlaylistItem::type() const
{
    return type_;
}

void KodiPlaylistItem::setType(const QString &type)
{
    type_ = type;
    emit typeChanged();
}

QString KodiPlaylistItem::file() const
{
    return file_;
}

void KodiPlaylistItem::setFile(const QString &file)
{
    file_ = file;
    emit fileChanged();
}

QString KodiPlaylistItem::label() const
{
    return label_;
}

void KodiPlaylistItem::setLabel(const QString &label)
{
    label_ = label;
    emit labelChanged();
}

int KodiPlaylistItem::artistId() const
{
    return artistId_;
}

void KodiPlaylistItem::setArtistId(int artistId)
{
    artistId_ = artistId;
    emit artistIdChanged();
}

int KodiPlaylistItem::albumId() const
{
    return albumId_;
}

void KodiPlaylistItem::setAlbumId(int albumId)
{
    albumId_ = albumId;
    emit albumIdChanged();
}

KodiPlaylistItem::KodiPlaylistItem(QObject *parent) : QObject(parent)
{

}

