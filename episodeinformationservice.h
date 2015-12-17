#ifndef EPISODEINFORMATIONSERVICE_H
#define EPISODEINFORMATIONSERVICE_H

#include <QObject>
#include <QDateTime>

class EpisodeInformationService : public QObject
{
    Q_OBJECT
    int episodeId_;
    QString plot_;
    double rating_;
    int runtime_;
    int episode_;
    QString showTitle_;
    QString file_;
    QString fanart_;
    QString thumbnail_;
    QDateTime lastplayed_;
    int season_;
    QString art_;


    QString title_;

public:
    explicit EpisodeInformationService(QObject *parent = 0);

    Q_PROPERTY(int episodeId READ episodeId WRITE setEpisodeId NOTIFY episodeIdChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString plot READ plot WRITE setPlot NOTIFY plotChanged)
    Q_PROPERTY(double rating READ rating WRITE setRating NOTIFY ratingChanged)
    Q_PROPERTY(int runtime READ runtime WRITE setRuntime NOTIFY runtimeChanged)
    Q_PROPERTY(int episode READ episode WRITE setEpisode NOTIFY episodeChanged)
    Q_PROPERTY(QString showTitle READ showTitle WRITE setShowTitle NOTIFY showTitleChanged)
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString fanart READ fanart WRITE setFanart NOTIFY fanartChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(QDateTime lastplayed READ lastplayed WRITE setLastplayed NOTIFY lastplayedChanged)
    Q_PROPERTY(int season READ season WRITE setSeason NOTIFY seasonChanged)
    Q_PROPERTY(QString art READ art WRITE setArt NOTIFY artChanged)

    QString plot() const;

    double rating() const;

    int runtime() const;

    int episode() const;

    QString showTitle() const;

    QString file() const;

    QString fanart() const;

    QString thumbnail() const;

    QDateTime lastplayed() const;

    int season() const;

    QString art() const;

    int episodeId() const;

    QString title() const;

signals:

    void plotChanged(QString plot);

    void ratingChanged(double rating);

    void runtimeChanged(int runtime);

    void episodeChanged(int episode);

    void showTitleChanged(QString showTitle);

    void fileChanged(QString file);

    void fanartChanged(QString fanart);

    void thumbnailChanged(QString thumbnail);

    void lastplayedChanged(QDateTime lastplayed);

    void seasonChanged(int season);

    void artChanged(QString art);

    void episodeIdChanged(int episodeId);

    void titleChanged(QString title);

public slots:
    void setPlot(QString plot);
    void setRating(double rating);
    void setRuntime(int runtime);
    void setEpisode(int episode);
    void setShowTitle(QString showTitle);
    void setFile(QString file);
    void setFanart(QString fanart);
    void setThumbnail(QString thumbnail);
    void setLastplayed(QDateTime lastplayed);
    void setSeason(int season);
    void setArt(QString art);
    void setEpisodeId(int episodeId);

    void refresh();
    void setTitle(QString title);

    void playFile();

private slots:
    void handleRefresh_();
};

        #endif // EPISODEINFORMATIONSERVICE_H
