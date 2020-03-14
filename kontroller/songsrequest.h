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
    explicit SongsRequest(QObject *parent = nullptr);
    ~SongsRequest();
    void start(int albumid);

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
