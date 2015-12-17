#ifndef EPISODESREQUEST_H
#define EPISODESREQUEST_H

#include <QObject>

class KodiFile;

class TvShowEpisodesRequest : public QObject
{
    Q_OBJECT
public:
    explicit TvShowEpisodesRequest(QObject *parent = 0);
    ~TvShowEpisodesRequest();

    std::vector<KodiFile*> episodes;
    bool success;

    /**
     * @brief start
     * @param tvshowId
     * @param season Season, not the season Id !!! (1 for first season, 2 for second, etc)
     */
    void start(int tvshowId, int season);
signals:
    void finished();
private slots:
    void parseEpisodesResult_();
public slots:
};

#endif // EPISODESREQUEST_H
