#ifndef EU_TGCM_KONTROLLER_PLAYLISTITEM_H
#define EU_TGCM_KONTROLLER_PLAYLISTITEM_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class PlaylistItem
{
	Q_GADGET
	/**
	 * @see type()
	 */
	QString type_;
	QString file_;
	QString label_;
	int artistId_ = 0;
	int albumId_ = 0;
	int movieId_ = 0;
	int episodeId_ = 0;
	int musicvideoId_ = 0;
	int songId_ = 0;
	int tvshowId_ = 0;
	QString artist_;
	QString album_;
	QString fanart_;
	QString thumbnail_;
	QString tvshow_;
	int duration_ = 0;

  public:
	PlaylistItem() = default;
	~PlaylistItem() = default;
	PlaylistItem(PlaylistItem const&) = default;
	PlaylistItem(PlaylistItem&&) = default;
	PlaylistItem& operator=(PlaylistItem const&) = default;
	PlaylistItem& operator=(PlaylistItem&&) = default;

	Q_PROPERTY(QString type READ type WRITE setType)
	Q_PROPERTY(QString file READ file WRITE setFile)
	Q_PROPERTY(QString label READ label WRITE setLabel)
	Q_PROPERTY(int artistId READ artistId WRITE setArtistId)
	Q_PROPERTY(int albumId READ albumId WRITE setAlbumId)
	Q_PROPERTY(int movieId READ movieId WRITE setMovieId)
	Q_PROPERTY(int episodeId READ episodeId WRITE setEpisodeId)
	Q_PROPERTY(int musicvideoId READ musicvideoId WRITE setMusicvideoId)
	Q_PROPERTY(int songId READ songId WRITE setSongId)
	Q_PROPERTY(int tvshowId READ tvshowId WRITE setTvshowId)
	Q_PROPERTY(QString artist READ artist WRITE setArtist)
	Q_PROPERTY(QString album READ album WRITE setAlbum)
	Q_PROPERTY(QString fanart READ fanart WRITE setFanart)
	Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail)
	Q_PROPERTY(QString tvshow READ tvshow WRITE setTvshow)
	Q_PROPERTY(int duration READ duration WRITE setDuration)
	/**
	 * @brief type the type of the playlist list item, may be photo, video or audio
	 * @return
	 */
	QString type() const;
	void setType(const QString& type);

	QString file() const;
	void setFile(const QString& file);

	QString label() const;
	void setLabel(const QString& label);

	int artistId() const;
	void setArtistId(int artistId);

	int albumId() const;
	void setAlbumId(int albumId);

	int movieId() const;

	int episodeId() const;

	int musicvideoId() const;

	QString artist() const;

	QString album() const;

	QString fanart() const;

	QString thumbnail() const;

	int songId() const;

	QString tvshow() const;

	int tvshowId() const;

	void setMovieId(int movieId);
	void setEpisodeId(int episodeId);
	void setMusicvideoId(int musicvideoId);
	void setArtist(QString artist);
	void setAlbum(QString album);
	void setFanart(QString fanart);
	void setThumbnail(QString thumbnail);
	void setSongId(int songId);
	void setTvshow(QString tvshow);
	void setTvshowId(int tvshowId);
	int duration() const;
	void setDuration(int duration);

	friend QDataStream& readFromStream(QDataStream&, eu::tgcm::kontroller::PlaylistItem& file);
  public slots:
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu

QDataStream& operator<<(QDataStream& stream, eu::tgcm::kontroller::PlaylistItem const& file);

QDataStream& operator>>(QDataStream& stream, eu::tgcm::kontroller::PlaylistItem& file);

Q_DECLARE_METATYPE(eu::tgcm::kontroller::PlaylistItem)

#endif // EU_TGCM_KONTROLLER_PLAYLISTITEM_H
