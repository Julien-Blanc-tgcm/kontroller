#ifndef EU_TGCM_KONTROLLER_TVSHOWSEASONSREQUEST_H
#define EU_TGCM_KONTROLLER_TVSHOWSEASONSREQUEST_H

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
class TvShowSeasonsRequest : public QObject
{
	Q_OBJECT
	eu::tgcm::kontroller::Client* client_ = nullptr;

public:
	explicit TvShowSeasonsRequest(Client* client, QObject *parent = nullptr);

	~TvShowSeasonsRequest() noexcept override;

	void start(int tvshowid);

	QVector<File> seasons;
	bool success;

signals:
	void finished();

private slots:
	void parseSeasonsResult_(int tvshowid);

};

}
}
}
#endif // EU_TGCM_KONTROLLER_TVSHOWSEASONSREQUEST_H
