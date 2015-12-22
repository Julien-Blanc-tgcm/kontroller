#ifndef EU_TGCM_KONTROLLER_SEASONINFORMATIONSERVICE_H
#define EU_TGCM_KONTROLLER_SEASONINFORMATIONSERVICE_H

#include <QObject>
#include <QQmlListProperty>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class File;

class SeasonInformationService : public QObject
{
    Q_OBJECT
    QString seasonId_; // contains both tvshowid and season, separated by |

    QString showTitle_;
    QString thumbnail_;
    double rating_;
    int nbEpisodes_;
    int nbWatchedEpisodes_;
    QString fanart_;
    QString art_;

    std::vector<File*> episodes_;

public:
    explicit SeasonInformationService(QObject *parent = 0);
    Q_PROPERTY(QString seasonId READ seasonId WRITE setSeasonId NOTIFY seasonIdChanged)
    Q_PROPERTY(QString showTitle READ showTitle WRITE setShowTitle NOTIFY showTitleChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(int nbEpisodes READ nbEpisodes WRITE setNbEpisodes NOTIFY nbEpisodesChanged)
    Q_PROPERTY(int nbWatchedEpisodes READ nbWatchedEpisodes WRITE setNbWatchedEpisodes NOTIFY nbWatchedEpisodesChanged)
    Q_PROPERTY(QString fanart READ fanart WRITE setFanart NOTIFY fanartChanged)
    Q_PROPERTY(QString art READ art WRITE setArt NOTIFY artChanged)
    Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::File> episodes READ episodes NOTIFY episodesChanged)
    Q_PROPERTY(QString season READ season NOTIFY seasonIdChanged)

    QString seasonId() const;
    void setSeasonId(const QString &seasonId);

    QString showTitle() const;
    void setShowTitle(const QString &title);

    int nbEpisodes() const;
    void setNbEpisodes(int nbEpisodes);

    int nbWatchedEpisodes() const;
    void setNbWatchedEpisodes(int nbWatchedEpisodes);

    QString fanart() const;
    void setFanart(const QString &fanart);

    QString art() const;
    void setArt(const QString &art);

    QQmlListProperty<File> episodes();

    QString thumbnail() const;
    void setThumbnail(const QString& thumbnail);

    QString season() const;

signals:
    void seasonIdChanged();
    void showTitleChanged();
    void thumbnailChanged();
    void nbEpisodesChanged();
    void nbWatchedEpisodesChanged();
    void fanartChanged();
    void artChanged();
    void episodesChanged();

public slots:
    void refresh();

private slots:
    void refreshEpisodes_();
    void handleRefresh_();
    void handleRefreshEpisodes_();
};

}
}
}
#endif // EU_TGCM_KONTROLLER_SEASONINFORMATIONSERVICE_H
