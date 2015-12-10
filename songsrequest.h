#ifndef SONGSREQUEST_H
#define SONGSREQUEST_H

#include <QObject>

class KodiFile;

class SongsRequest : public QObject
{
    Q_OBJECT
public:
    explicit SongsRequest(QObject *parent = 0);
    ~SongsRequest();
    void start(int artistid);

    QList<KodiFile*> results;
    bool success;
signals:
    void finished();
private slots:
    void parseSongsResult();
};

#endif // SONGSREQUEST_H
