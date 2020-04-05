#ifndef EU_TGCM_KONTROLLER_ALBUMSREQUEST_H
#define EU_TGCM_KONTROLLER_ALBUMSREQUEST_H

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
	explicit AlbumsRequest(QObject *parent = 0);
	~AlbumsRequest();
	void start(int artistid);
	void startWithGenre(int genreid);

	QList<File*> results;
	bool success;
signals:
	void finished();
private slots:
	void parseAlbumsResult();
};

}
}
}
#endif // EU_TGCM_KONTROLLER_ALBUMSREQUEST_H
