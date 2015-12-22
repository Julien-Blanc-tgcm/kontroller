#ifndef EU_TGCM_KONTROLLER_VIDEOCONTROL_H
#define EU_TGCM_KONTROLLER_VIDEOCONTROL_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class File;

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
    void addToPlaylist(File* file);
    void playFile(File* file);
    void startPlaying();
};

}
}
}
#endif // EU_TGCM_KONTROLLER_VIDEOCONTROL_H
