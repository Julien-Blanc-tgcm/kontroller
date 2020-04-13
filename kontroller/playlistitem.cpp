#include "playlistitem.h"

#include <QDataStream>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

QString PlaylistItem::type() const
{
	return type_;
}

void PlaylistItem::setType(const QString &type)
{
	type_ = type;
}

QString PlaylistItem::file() const
{
	return file_;
}

void PlaylistItem::setFile(const QString &file)
{
	file_ = file;
}

QString PlaylistItem::label() const
{
	return label_;
}

void PlaylistItem::setLabel(const QString &label)
{
	label_ = label;
}

int PlaylistItem::artistId() const
{
	return artistId_;
}

void PlaylistItem::setArtistId(int artistId)
{
	artistId_ = artistId;
}

int PlaylistItem::albumId() const
{
	return albumId_;
}

void PlaylistItem::setAlbumId(int albumId)
{
	albumId_ = albumId;
}

int PlaylistItem::movieId() const
{
	return movieId_;
}

int PlaylistItem::episodeId() const
{
	return episodeId_;
}

int PlaylistItem::musicvideoId() const
{
	return musicvideoId_;
}

QString PlaylistItem::artist() const
{
	return artist_;
}

QString PlaylistItem::album() const
{
	return album_;
}

QString PlaylistItem::fanart() const
{
	return fanart_;
}

QString PlaylistItem::thumbnail() const
{
	return thumbnail_;
}

int PlaylistItem::songId() const
{
	return songId_;
}

QString PlaylistItem::tvshow() const
{
	return tvshow_;
}

void PlaylistItem::setMovieId(int movieId)
{
	if (movieId_ == movieId)
		return;

	movieId_ = movieId;
}

void PlaylistItem::setEpisodeId(int episodeId)
{
	if (episodeId_ == episodeId)
		return;

	episodeId_ = episodeId;
}

void PlaylistItem::setMusicvideoId(int musicvideoId)
{
	if (musicvideoId_ == musicvideoId)
		return;

	musicvideoId_ = musicvideoId;
}

void PlaylistItem::setArtist(QString artist)
{
	artist_ = artist;
}

void PlaylistItem::setAlbum(QString album)
{
	album_ = album;
}

void PlaylistItem::setFanart(QString fanart)
{
	fanart_ = fanart;
}

void PlaylistItem::setThumbnail(QString thumbnail)
{
	thumbnail_ = thumbnail;
}

void PlaylistItem::setSongId(int songId)
{
	songId_ = songId;
}

void PlaylistItem::setTvshow(QString tvshow)
{
	tvshow_ = tvshow;
}

QDataStream& readFromStream(QDataStream& stream, eu::tgcm::kontroller::PlaylistItem& file)
{
	stream >> file.album_;
	stream >> file.albumId_;
	stream >> file.artist_;
	stream >> file.artistId_;
	stream >> file.episodeId_;
	stream >> file.fanart_;
	stream >> file.file_;
	stream >> file.label_;
	stream >> file.movieId_;
	stream >> file.musicvideoId_;
	stream >> file.songId_;
	return stream;
}


}
}
}

QDataStream& operator<<(QDataStream& stream, const eu::tgcm::kontroller::PlaylistItem& file)
{
	return stream << file.album() << file.albumId() << file.artist() << file.artistId() << file.episodeId()
	              << file.fanart() << file.file() << file.label() << file.movieId() << file.musicvideoId()
	              << file.songId();
}

QDataStream& operator>>(QDataStream& stream, eu::tgcm::kontroller::PlaylistItem& file)
{
	return eu::tgcm::kontroller::readFromStream(stream, file);
}

