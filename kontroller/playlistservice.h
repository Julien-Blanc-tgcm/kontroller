#ifndef EU_TGCM_KONTROLLER_PLAYLISTSERVICE_H
#define EU_TGCM_KONTROLLER_PLAYLISTSERVICE_H

#include "playlistitem.h"

#include <QObject>
#include <QQmlListProperty>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class Client;
/**
 * @brief The PlaylistService class handles the current playlist related operations
 */
class PlaylistService : public QObject
{
	Q_OBJECT

	eu::tgcm::kontroller::Client* client_ = nullptr;
	QList<PlaylistItem*> currentItems_;
	int playlistId_;
	QString playlistType_;
	int playlistPosition_;

	bool refreshing_ = false;
	bool restartAfterRefreshing_ = false;

	int pendingPosition_ = -1;

public:
	explicit PlaylistService(QObject *parent = nullptr);

	Q_PROPERTY(int playlistId READ playlistId WRITE setPlaylistId NOTIFY playlistIdChanged)
	Q_PROPERTY(QString playlistType READ playlistType WRITE setPlaylistType NOTIFY playlistTypeChanged)
	Q_PROPERTY(int playlistPosition READ playlistPosition WRITE setPlaylistPosition NOTIFY playlistPositionChanged)
	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::PlaylistItem> items READ items NOTIFY itemsChanged)

	int playlistId() const;
	void setPlaylistId(int playlistId);

	QString playlistType() const;
	void setPlaylistType(const QString &playlistType);

	int playlistPosition() const;
	void setPlaylistPosition(int playlistPosition);

	QList<PlaylistItem*> const currentItems() const;
	QQmlListProperty<PlaylistItem> items();

	eu::tgcm::kontroller::Client* client() const;

signals:
	void playlistIdChanged();
	void itemsChanged();
	void playlistTypeChanged();
	void playlistPositionChanged();
	void clientChanged(eu::tgcm::kontroller::Client* client);

public slots:
	void switchToItem(int position);
	void clearPlaylist();
	void removeElement(int index);
	/**
	 * @brief refresh refreshes the default playlist from the server
	 */
	void refresh();
	void setClient(eu::tgcm::kontroller::Client* client);

private slots:
	void setCurrentlyPlayedItem_(int playerId, QString type, int id);
	void refreshPlaylist_();
	void refreshPlaylistCb_();
	void findMatchingPlaylist_();
	void findMatchingPlaylistCb_();
	void handlePlaylistCleared_(int playlist);
	void handlePlaylistElementRemoved(int playlist, int position);
	void handlePlaylistElementAdded(int playlistId);
	void handlePlayerChanged_();
	void handleGotoReply_();
};

}
}
}

#endif // EU_TGCM_KONTROLLER_PLAYLISTSERVICE_H
