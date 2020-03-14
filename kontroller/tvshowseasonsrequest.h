#ifndef EU_TGCM_KONTROLLER_TVSHOWSEASONSREQUEST_H
#define EU_TGCM_KONTROLLER_TVSHOWSEASONSREQUEST_H

#include <QObject>
#include <vector>

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
public:
    explicit TvShowSeasonsRequest(QObject *parent = 0);
    ~TvShowSeasonsRequest();
    void start(int tvshowid);

    std::vector<File*> seasons;
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
