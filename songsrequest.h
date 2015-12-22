#ifndef EU_TGCM_KONTROLLER_SONGSREQUEST_H
#define EU_TGCM_KONTROLLER_SONGSREQUEST_H

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
    explicit SongsRequest(QObject *parent = 0);
    ~SongsRequest();
    void start(int artistid);

    QList<File*> results;
    bool success;
signals:
    void finished();
private slots:
    void parseSongsResult();
};

}
}
}
#endif // EU_TGCM_KONTROLLER_SONGSREQUEST_H
