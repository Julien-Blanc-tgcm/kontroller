#ifndef MUSICCONTROL_H
#define MUSICCONTROL_H

#include <QObject>

class KodiFile;

class MusicControl : public QObject
{
    Q_OBJECT

private:
    int audioPlaylistId_ = 0;
public:
    explicit MusicControl(QObject *parent = 0);

signals:

public slots:
    void playFile(KodiFile* file);
    void addToPlaylist(KodiFile* file);
    void startPlaying();
    void clearPlaylist();
};

#endif // MUSICCONTROL_H
