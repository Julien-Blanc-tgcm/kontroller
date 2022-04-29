#ifndef EU_TGCM_KONTROLLER_PLAYLISTREQUEST_HPP
#define EU_TGCM_KONTROLLER_PLAYLISTREQUEST_HPP

#include <QObject>

#include "../client.h"
#include "../file.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class PlaylistRequest : public QObject
{
	Q_OBJECT
  public:
	explicit PlaylistRequest(Client* client, QObject* parent = nullptr);
	~PlaylistRequest() noexcept override;
	PlaylistRequest(PlaylistRequest&&) = delete;
	PlaylistRequest(PlaylistRequest const&) = delete;
	PlaylistRequest& operator=(PlaylistRequest&&) = delete;
	PlaylistRequest& operator=(PlaylistRequest const&) = delete;

	void start();

	void startWithPlaylist(QString playlist);

	QVector<File> results;
	bool success;
  signals:
	void finished();
  private slots:
	void parsePlaylistsResult_();

  private:
	eu::tgcm::kontroller::Client* client_ = nullptr;

  signals:
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu

#endif // EU_TGCM_KONTROLLER_PLAYLISTREQUEST_HPP
