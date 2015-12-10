#ifndef VIDEOCONTROL_H
#define VIDEOCONTROL_H

#include <QObject>

class KodiFile;

class VideoControl : public QObject
{
    Q_OBJECT
private:
    int videoPlaylistId_ = 1;
public:
    explicit VideoControl(QObject *parent = 0);

signals:

public slots:
    void clearPlaylist();
    void addFileToPlaylist(KodiFile* file);
    void playFile(KodiFile* file);
    void startPlaying();
};

#endif // VIDEOCONTROL_H
