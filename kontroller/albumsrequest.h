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
	explicit AlbumsRequest(Client* client, QObject *parent = nullptr);
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

	QVector<File> results;
	Client* client_ = nullptr;
	bool success;
signals:
	void finished();
private slots:
	void parseAlbumsResult();
private:
};

}
}
}
#endif // EU_TGCM_KONTROLLER_ALBUMSREQUEST_H
