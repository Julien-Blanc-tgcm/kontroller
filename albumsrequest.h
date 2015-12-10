#ifndef ALBUMSREQUEST_H
#define ALBUMSREQUEST_H

#include <QObject>

class KodiFile;

class AlbumsRequest : public QObject
{
    Q_OBJECT
public:
    explicit AlbumsRequest(QObject *parent = 0);
    ~AlbumsRequest();
    void start(int artistid);

    QList<KodiFile*> results;
    bool success;
signals:
    void finished();
private slots:
    void parseAlbumsResult();
};

#endif // ALBUMSREQUEST_H
