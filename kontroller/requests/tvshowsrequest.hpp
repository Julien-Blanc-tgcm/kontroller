#ifndef TVSHOWSREQUEST_HPP
#define TVSHOWSREQUEST_HPP

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

class TvShowsRequest : public QObject
{
	Q_OBJECT
  public:
	TvShowsRequest(Client* client, QObject* parent = nullptr);
	~TvShowsRequest() noexcept = default;
	TvShowsRequest(TvShowsRequest const&) = delete;
	TvShowsRequest(TvShowsRequest&&) = delete;
	TvShowsRequest& operator=(TvShowsRequest const&) = delete;
	TvShowsRequest& operator=(TvShowsRequest&&) = delete;

	bool success = false;
	QVector<File> results;

	void start();

  signals:
	void finished();
  private slots:
	void parseResult_();

  private:
	Client* client_ = nullptr;

};

} // namespace kontroller
} // namespace tgcm
} // namespace eu
#endif // TVSHOWSREQUEST_HPP
