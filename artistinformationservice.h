#ifndef ARTISTINFORMATIONSERVICE_H
#define ARTISTINFORMATIONSERVICE_H

#include <QObject>
#include <kodifile.h>
#include <QQmlListProperty>

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
    QList<KodiFile*> albums_;
public:
    explicit ArtistInformationService(QObject *parent = 0);
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
    Q_PROPERTY(QString artistName READ artistName WRITE setArtistName NOTIFY artistNameChanged)
    Q_PROPERTY(QString artistDescription READ artistDescription WRITE setArtistDescription NOTIFY artistDescriptionChanged)
    Q_PROPERTY(QString fanart READ fanart WRITE setFanart NOTIFY fanartChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QString genres READ genres NOTIFY genresChanged)
    Q_PROPERTY(QString style READ style NOTIFY styleChanged)
    Q_PROPERTY(QQmlListProperty<KodiFile> albums READ albums NOTIFY albumsChanged)

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

    QQmlListProperty<KodiFile> albums();

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

#endif // ARTISTINFORMATIONSERVICE_H
