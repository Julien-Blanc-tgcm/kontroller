#ifndef EU_TGCM_KONTROLLER_ALBUMINFORMATIONSERVICE_H
#define EU_TGCM_KONTROLLER_ALBUMINFORMATIONSERVICE_H

#include "client.h"

#include "file.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class MusicControl;

class AlbumInformationService : public QObject
{
	Q_OBJECT
private:
	int albumId_ = 0;
	QString name_;
	QString description_;
	QString thumbnail_;
	QVector<File> songs_;
	QStringList genres_;
	int year_ = 0;
	QString label_;
	QStringList artists_;
	eu::tgcm::kontroller::Client* client_ = nullptr;
	MusicControl* ctrl_ = nullptr;

public:
	explicit AlbumInformationService(QObject* parent = nullptr);
	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	Q_PROPERTY(int albumId READ albumId WRITE setAlbumId NOTIFY albumIdChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
	Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
	Q_PROPERTY(QVariantList songs READ songs NOTIFY songsChanged)
	Q_PROPERTY(QStringList genres READ genres WRITE setGenres NOTIFY genresChanged)
	Q_PROPERTY(int year READ year WRITE setYear NOTIFY yearChanged)
	Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
	Q_PROPERTY(QStringList artists READ artists WRITE setArtists NOTIFY artistsChanged)

	int albumId() const;
	void setAlbumId(int albumId);
	QString name() const;
	void setName(const QString &name);
	QString description() const;
	void setDescription(const QString &description);
	QString thumbnail() const;
	void setThumbnail(const QString &value);

	QVariantList songs();

	QStringList genres() const;
	void setGenres(const QStringList &genre);

	int year() const;
	void setYear(const int &year);

	QString label() const;
	void setLabel(const QString &label);

	QStringList artists() const;
	void setArtists(const QStringList &artists);

	eu::tgcm::kontroller::Client* client() const;

	Q_INVOKABLE void playFile();

public slots:
	void refresh();

	void setClient(eu::tgcm::kontroller::Client* client);

signals:
	void albumIdChanged();
	void nameChanged();
	void descriptionChanged();
	void thumbnailChanged();
	void songsChanged();
	void genresChanged();
	void yearChanged();
	void labelChanged();
	void artistsChanged();
	void clientChanged(eu::tgcm::kontroller::Client* client);

private slots:
	void handleRefresh_();
	void handleSongs_();
};

}
}
}

#endif // EU_TGCM_KONTROLLER_ALBUMINFORMATIONSERVICE_H
