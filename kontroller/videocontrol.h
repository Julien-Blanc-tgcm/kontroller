#ifndef EU_TGCM_KONTROLLER_VIDEOCONTROL_H
#define EU_TGCM_KONTROLLER_VIDEOCONTROL_H

#include "client.h"
#include "file.h"

#include <QObject>
#include <qjsonrpc/qjsonrpcservicereply.h>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class VideoControl : public QObject
{
	Q_OBJECT
private:
	int videoPlaylistId_ = 1;
	eu::tgcm::kontroller::Client* client_ = nullptr;
	File currentFile_;
public:
	explicit VideoControl(QObject *parent = nullptr);

	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	eu::tgcm::kontroller::Client* client() const;

signals:
	void clientChanged(eu::tgcm::kontroller::Client* client);

public slots:
	QJsonRpcServiceReply *clearPlaylist();
	QJsonRpcServiceReply *addToPlaylist(eu::tgcm::kontroller::File file);
	void playFile(eu::tgcm::kontroller::File file);
	void resumeFile(eu::tgcm::kontroller::File file, int position);
	void startPlaying();

	void setClient(eu::tgcm::kontroller::Client* client);

private slots:
	void addCurrentFileToPlaylist_();
};

}
}
}
#endif // EU_TGCM_KONTROLLER_VIDEOCONTROL_H
