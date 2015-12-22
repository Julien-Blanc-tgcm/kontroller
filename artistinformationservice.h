#ifndef EU_TGCM_KONTROLLER_ARTISTINFORMATIONSERVICE_H
#define EU_TGCM_KONTROLLER_ARTISTINFORMATIONSERVICE_H

#include <QObject>
#include <file.h>
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
    QList<File*> albums_;
public:
    explicit ArtistInformationService(QObject *parent = 0);
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
    Q_PROPERTY(QString artistName READ artistName WRITE setArtistName NOTIFY artistNameChanged)
    Q_PROPERTY(QString artistDescription READ artistDescription WRITE setArtistDescription NOTIFY artistDescriptionChanged)
    Q_PROPERTY(QString fanart READ fanart WRITE setFanart NOTIFY fanartChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QString genres READ genres NOTIFY genresChanged)
    Q_PROPERTY(QString style READ style NOTIFY styleChanged)
    Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::File> albums READ albums NOTIFY albumsChanged)

    int artistId() const;
    void setArtistId(int artistId);

    QString artistName() const;
    void setArtistName(const QString &artistName);

    QString artistDescription() const;
    void setArtistDescription(const QString &artistDescription);

    QString fanart() const;
    void setFanart(const QString &fanart);

    QString thumbnail() const;
    void setThumbnail(const QString &thumbnail);

    QString genres() const;

    QString style() const;

    QQmlListProperty<File> albums();

signals:
    void artistIdChanged();
    void artistNameChanged();
    void artistDescriptionChanged();
    void fanartChanged();
    void thumbnailChanged();
    void genresChanged();
    void styleChanged();
    void albumsChanged();
public slots:
    void refresh();
private slots:
    void handleRefresh_();
    void handleAlbums_();
};
}
}
}
#endif // EU_TGCM_KONTROLLER_ARTISTINFORMATIONSERVICE_H
