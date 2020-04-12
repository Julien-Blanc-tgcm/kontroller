#ifndef EU_TGCM_KONTROLLER_EPISODESREQUEST_H
#define EU_TGCM_KONTROLLER_EPISODESREQUEST_H

#include "client.h"

#include <QObject>
#include <QVector>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class File;

class TvShowEpisodesRequest : public QObject
{
    Q_OBJECT
public:
	explicit TvShowEpisodesRequest(Client* client, QObject *parent = 0);
    ~TvShowEpisodesRequest();

	QVector<File> episodes;
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
private:
	Client* client_ = nullptr;
};

}
}
}

#endif // EU_TGCM_KONTROLLER_EPISODESREQUEST_H
