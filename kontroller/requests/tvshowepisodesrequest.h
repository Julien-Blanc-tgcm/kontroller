#ifndef EU_TGCM_KONTROLLER_EPISODESREQUEST_H
#define EU_TGCM_KONTROLLER_EPISODESREQUEST_H

#include <QObject>
#include <QVector>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Client;
class File;

class TvShowEpisodesRequest : public QObject
{
	Q_OBJECT
  public:
	explicit TvShowEpisodesRequest(Client* client, QObject* parent = nullptr);
	~TvShowEpisodesRequest() noexcept override;
	TvShowEpisodesRequest(TvShowEpisodesRequest&&) = delete;
	TvShowEpisodesRequest(TvShowEpisodesRequest const&) = delete;
	TvShowEpisodesRequest& operator=(TvShowEpisodesRequest&&) = delete;
	TvShowEpisodesRequest& operator=(TvShowEpisodesRequest const&) = delete;

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

} // namespace kontroller
} // namespace tgcm
} // namespace eu

#endif // EU_TGCM_KONTROLLER_EPISODESREQUEST_H
