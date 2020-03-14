#ifndef EU_TGCM_KONTROLLER_PLAYLISTITEM_H
#define EU_TGCM_KONTROLLER_PLAYLISTITEM_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class PlaylistItem : public QObject
{
	Q_OBJECT
	/**
	 * @see type()
	 */
	QString type_;
	QString file_;
	QString label_;
	int artistId_;
	int albumId_;
	int movieId_;
	int episodeId_;
	int musicvideoId_;
	int songId_;
	QString artist_;
	QString album_;
	QString fanart_;
	QString thumbnail_;
	QString tvshow_;



public:
    explicit PlaylistItem(QObject *parent = 0);

    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
    Q_PROPERTY(int albumId READ albumId WRITE setAlbumId NOTIFY albumIdChanged)
    Q_PROPERTY(int movieId READ movieId WRITE setMovieId NOTIFY movieIdChanged)
    Q_PROPERTY(int episodeId READ episodeId WRITE setEpisodeId NOTIFY episodeIdChanged)
    Q_PROPERTY(int musicvideoId READ musicvideoId WRITE setMusicvideoId NOTIFY musicvideoIdChanged)
    Q_PROPERTY(int songId READ songId WRITE setSongId NOTIFY songIdChanged)
    Q_PROPERTY(QString artist READ artist WRITE setArtist NOTIFY artistChanged)
    Q_PROPERTY(QString album READ album WRITE setAlbum NOTIFY albumChanged)
    Q_PROPERTY(QString fanart READ fanart WRITE setFanart NOTIFY fanartChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QString tvshow READ tvshow WRITE setTvshow NOTIFY tvshowChanged)

    /**
     * @brief type the type of the playlist list item, may be photo, video or audio
     * @return
     */
    QString type() const;
    void setType(const QString &type);

    QString file() const;
    void setFile(const QString &file);

    QString label() const;
    void setLabel(const QString &label);

    int artistId() const;
    void setArtistId(int artistId);

    int albumId() const;
    void setAlbumId(int albumId);

    int movieId() const
    {
        return movieId_;
    }

    int episodeId() const
    {
        return episodeId_;
    }

    int musicvideoId() const
    {
        return musicvideoId_;
    }

    QString artist() const
    {
        return artist_;
    }

    QString album() const
    {
        return album_;
    }

    QString fanart() const
    {
        return fanart_;
    }

    QString thumbnail() const
    {
        return thumbnail_;
    }

    int songId() const
    {
        return songId_;
    }

    QString tvshow() const
    {
        return tvshow_;
    }

signals:
    void typeChanged();
    void fileChanged();
    void labelChanged();
    void artistIdChanged();
    void albumIdChanged();
    void movieIdChanged(int movieId);

    void episodeIdChanged(int episodeId);

    void musicvideoIdChanged(int musicvideoId);

    void artistChanged(QString artist);

    void albumChanged(QString album);

    void fanartChanged(QString fanart);

    void thumbnailChanged(QString thumbnail);

    void songIdChanged(int songId);

    void tvshowChanged(QString tvshow);

public slots:
void setMovieId(int movieId)
{
    if (movieId_ == movieId)
        return;

    movieId_ = movieId;
    emit movieIdChanged(movieId);
}
void setEpisodeId(int episodeId)
{
    if (episodeId_ == episodeId)
        return;

    episodeId_ = episodeId;
    emit episodeIdChanged(episodeId);
}
void setMusicvideoId(int musicvideoId)
{
    if (musicvideoId_ == musicvideoId)
        return;

    musicvideoId_ = musicvideoId;
    emit musicvideoIdChanged(musicvideoId);
}
void setArtist(QString artist)
{
    if (artist_ == artist)
        return;

    artist_ = artist;
    emit artistChanged(artist);
}
void setAlbum(QString album)
{
    if (album_ == album)
        return;

    album_ = album;
    emit albumChanged(album);
}
void setFanart(QString fanart)
{
    if (fanart_ == fanart)
        return;

    fanart_ = fanart;
    emit fanartChanged(fanart);
}
void setThumbnail(QString thumbnail)
{
    if (thumbnail_ == thumbnail)
        return;

    thumbnail_ = thumbnail;
    emit thumbnailChanged(thumbnail);
}
void setSongId(int songId)
{
    if (songId_ == songId)
        return;

    songId_ = songId;
    emit songIdChanged(songId);
}
void setTvshow(QString tvshow)
{
    if (tvshow_ == tvshow)
        return;

    tvshow_ = tvshow;
    emit tvshowChanged(tvshow);
}
};

}
}
}
#endif // EU_TGCM_KONTROLLER_PLAYLISTITEM_H
