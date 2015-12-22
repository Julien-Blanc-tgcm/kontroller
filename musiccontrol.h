#ifndef EU_TGCM_KONTROLLER_MUSICCONTROL_H
#define EU_TGCM_KONTROLLER_MUSICCONTROL_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class File;

class MusicControl : public QObject
{
    Q_OBJECT

private:
    int audioPlaylistId_ = 0;
public:
    explicit MusicControl(QObject *parent = 0);

signals:

public slots:
    void playFile(File* file);
    void addToPlaylist(File* file);
    void startPlaying();
    void clearPlaylist();
};

}
}
}
#endif // EU_TGCM_KONTROLLER_MUSICCONTROL_H
