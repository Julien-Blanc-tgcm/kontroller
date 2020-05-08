#ifndef EU_TGCM_KONTROLLER_ARTISTINFORMATIONSERVICE_H
#define EU_TGCM_KONTROLLER_ARTISTINFORMATIONSERVICE_H

#include "client.h"
#include <file.h>

#include <QObject>
#include <QQmlListProperty>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class ArtistInformationService : public QObject
{
	Q_OBJECT

  private:
	int artistId_;
	QString artistName_;
	QString artistDescription_;
	QString fanart_;
	QString thumbnail_;
	QList<QString> genres_;
	QList<QString> style_;
	QVector<File> albums_;
	eu::tgcm::kontroller::Client* client_ = nullptr;

  public:
	explicit ArtistInformationService(QObject* parent = nullptr);
	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
	Q_PROPERTY(QString artistName READ artistName WRITE setArtistName NOTIFY artistNameChanged)
	Q_PROPERTY(
	    QString artistDescription READ artistDescription WRITE setArtistDescription NOTIFY artistDescriptionChanged)
	Q_PROPERTY(QString fanart READ fanart WRITE setFanart NOTIFY fanartChanged)
	Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
	Q_PROPERTY(QString genres READ genres NOTIFY genresChanged)
	Q_PROPERTY(QString style READ style NOTIFY styleChanged)
	Q_PROPERTY(QVariantList albums READ albums NOTIFY albumsChanged)

	int artistId() const;
	void setArtistId(int artistId);

	QString artistName() const;
	void setArtistName(const QString& artistName);

	QString artistDescription() const;
	void setArtistDescription(const QString& artistDescription);

	QString fanart() const;
	void setFanart(const QString& fanart);

	QString thumbnail() const;
	void setThumbnail(const QString& thumbnail);

	QString genres() const;

	QString style() const;

	QVariantList albums();

	eu::tgcm::kontroller::Client* client() const;

  signals:
	void artistIdChanged();
	void artistNameChanged();
	void artistDescriptionChanged();
	void fanartChanged();
	void thumbnailChanged();
	void genresChanged();
	void styleChanged();
	void albumsChanged();
	void clientChanged(eu::tgcm::kontroller::Client* client);

  public slots:
	void refresh();
	void setClient(eu::tgcm::kontroller::Client* client);

  private slots:
	void handleRefresh_();
	void handleAlbums_();
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu
#endif // EU_TGCM_KONTROLLER_ARTISTINFORMATIONSERVICE_H
