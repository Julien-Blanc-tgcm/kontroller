#ifndef EU_TGCM_KONTROLLER_ALBUMINFORMATIONSERVICE_H
#define EU_TGCM_KONTROLLER_ALBUMINFORMATIONSERVICE_H

#include <QQmlListProperty>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class File;

class AlbumInformationService : public QObject
{
    Q_OBJECT
private:
    int albumId_;
    QString name_;
    QString description_;
    QString thumbnail_;
    QList<File*> songs_;
    QString genre_;
    int year_;
    QString label_;
    QStringList artists_;
public:
    explicit AlbumInformationService(QObject* parent = nullptr);

    Q_PROPERTY(int albumId READ albumId WRITE setAlbumId NOTIFY albumIdChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::File> songs READ songs NOTIFY songsChanged)
    Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY genreChanged)
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

    QQmlListProperty<File> songs();

    QString genre() const;
    void setGenre(const QString &genre);

    int year() const;
    void setYear(const int &year);

    QString label() const;
    void setLabel(const QString &label);

    QStringList artists() const;
    void setArtists(const QStringList &artists);

public slots:
    void refresh();

signals:
    void albumIdChanged();
    void nameChanged();
    void descriptionChanged();
    void thumbnailChanged();
    void songsChanged();
    void genreChanged();
    void yearChanged();
    void labelChanged();
    void artistsChanged();
private slots:
    void handleRefresh_();
    void handleSongs_();
};

}
}
}

#endif // EU_TGCM_KONTROLLER_ALBUMINFORMATIONSERVICE_H
