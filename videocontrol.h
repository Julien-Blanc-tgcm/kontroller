#ifndef EU_TGCM_KONTROLLER_VIDEOCONTROL_H
#define EU_TGCM_KONTROLLER_VIDEOCONTROL_H

#include <QObject>
#include <qjsonrpcservicereply.h>

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
	File* currentFile_ = nullptr;
public:
	explicit VideoControl(QObject *parent = 0);

signals:

public slots:
	QJsonRpcServiceReply *clearPlaylist();
	QJsonRpcServiceReply *addToPlaylist(eu::tgcm::kontroller::File* file);
	void playFile(eu::tgcm::kontroller::File* file);
	void startPlaying();

private slots:
	void addCurrentFileToPlaylist_();

};

}
}
}
#endif // EU_TGCM_KONTROLLER_VIDEOCONTROL_H
