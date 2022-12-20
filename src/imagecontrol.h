#ifndef EU_TGCM_KONTROLLER_IMAGECONTROL_H
#define EU_TGCM_KONTROLLER_IMAGECONTROL_H

#include <QObject>

#include "client.h"
#include "file.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class ImageControl : public QObject
{
	Q_OBJECT
  private:
	int imagePlaylistId_ = 2;
	eu::tgcm::kontroller::Client* client_ = nullptr;

  public:
	explicit ImageControl(QObject* parent = nullptr);

	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)

	Client* client() const;

  signals:
	void clientChanged();
  public slots:
	void setClient(eu::tgcm::kontroller::Client* client);
	void playFile(eu::tgcm::kontroller::File file);
	void addToPlaylist(eu::tgcm::kontroller::File file);
	void startPlaying();
	void clearPlaylist();
};

}
}
} // namespace eu
#endif // EU_TGCM_KONTROLLER_IMAGECONTROL_H
