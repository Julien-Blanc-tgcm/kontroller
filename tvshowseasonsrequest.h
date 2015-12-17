#ifndef TVSHOWSEASONSREQUEST_H
#define TVSHOWSEASONSREQUEST_H

#include <QObject>
#include <vector>
class KodiFile;
class TvShowSeasonsRequest : public QObject
{
    Q_OBJECT
public:
    explicit TvShowSeasonsRequest(QObject *parent = 0);
    ~TvShowSeasonsRequest();
    void start(int tvshowid);

    std::vector<KodiFile*> seasons;
    bool success;
signals:
    void finished();
private slots:
    void parseSeasonsResult_(int tvshowid);

};

#endif // TVSHOWSEASONSREQUEST_H
