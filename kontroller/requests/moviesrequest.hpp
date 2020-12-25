#ifndef MOVIESREQUEST_HPP
#define MOVIESREQUEST_HPP

#include "client.h"
#include "file.h"

#include <QObject>
#include <QVector>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class File;

class MoviesRequest : public QObject
{
	Q_OBJECT
  public:
	explicit MoviesRequest(Client* client, QObject* parent = nullptr);
	~MoviesRequest() noexcept = default;
	MoviesRequest(MoviesRequest const&) = delete;
	MoviesRequest(MoviesRequest&&) = delete;
	MoviesRequest& operator=(MoviesRequest const&) = delete;
	MoviesRequest& operator=(MoviesRequest&&) = delete;

	void start();

	QVector<File> results;
	bool success = false;
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
#endif // MOVIESREQUEST_HPP
