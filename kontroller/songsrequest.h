#ifndef EU_TGCM_KONTROLLER_SONGSREQUEST_H
#define EU_TGCM_KONTROLLER_SONGSREQUEST_H

#include "client.h"

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class File;

class SongsRequest : public QObject
{
	Q_OBJECT
public:
	explicit SongsRequest(Client* client, QObject *parent = nullptr);
	~SongsRequest() override;
	SongsRequest(SongsRequest&&) = delete;
	SongsRequest(SongsRequest const&) = delete;
	SongsRequest& operator=(SongsRequest&&) = delete;
	SongsRequest& operator=(SongsRequest const&) = delete;

	void start(int albumid);

	QVector<File> results;
	bool success;
signals:
	void finished();
private slots:
	void parseSongsResult();
private:
	eu::tgcm::kontroller::Client* client_ = nullptr;
};

}
}
}
#endif // EU_TGCM_KONTROLLER_SONGSREQUEST_H
