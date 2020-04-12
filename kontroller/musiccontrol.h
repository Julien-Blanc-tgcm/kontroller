#ifndef EU_TGCM_KONTROLLER_MUSICCONTROL_H
#define EU_TGCM_KONTROLLER_MUSICCONTROL_H

#include "client.h"

#include "file.h"

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class MusicControl : public QObject
{
    Q_OBJECT

private:
	int audioPlaylistId_ = 0;
	eu::tgcm::kontroller::Client* client_ = nullptr;

public:
	explicit MusicControl(QObject *parent = 0);
	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	eu::tgcm::kontroller::Client* client() const;

signals:

	void clientChanged(eu::tgcm::kontroller::Client* client);

public slots:
	void playFile(eu::tgcm::kontroller::File file);
	void addToPlaylist(eu::tgcm::kontroller::File file);
	void startPlaying();
	void clearPlaylist();
	void setClient(eu::tgcm::kontroller::Client* client);
};

}
}
}
#endif // EU_TGCM_KONTROLLER_MUSICCONTROL_H
