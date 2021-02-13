#ifndef EU_TGCM_KONTROLLER_ALBUMSREQUEST_H
#define EU_TGCM_KONTROLLER_ALBUMSREQUEST_H

#include "client.h"

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class File;

class AlbumsRequest : public QObject
{
	Q_OBJECT
  public:
	explicit AlbumsRequest(Client* client, QObject* parent = nullptr);
	~AlbumsRequest() noexcept override;
	AlbumsRequest(AlbumsRequest&&) = delete;
	AlbumsRequest(AlbumsRequest const&) = delete;
	AlbumsRequest& operator=(AlbumsRequest&&) = delete;
	AlbumsRequest& operator=(AlbumsRequest const&) = delete;
	/**
	 * @brief start starts the request.
	 * @param artistid
	 */
	void start(int artistid);
	void startWithGenre(int genreid);

	/**
	 * @brief startRecentlyAdded starts a recently added albums request
	 */
	void startRecentlyAdded();

	/**
	 * @brief startRecentlyPlayed starts a recently added albums request
	 */
	void startRecentlyPlayed();

	/**
	 * @brief startRandom starts a random album request, that will retrieve the given number of items
	 * @param count The number of random albums to retrieve
	 */
	void startRandom(int count);

	/**
	 * @brief startFavorites starts a request to retrieve favorites albums
	 */
	void startFavorites();

	QVector<File> results;
	Client* client_ = nullptr;
	bool success;
  signals:
	void finished();
  private slots:
	void parseAlbumsResult();

  private:
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu
#endif // EU_TGCM_KONTROLLER_ALBUMSREQUEST_H
