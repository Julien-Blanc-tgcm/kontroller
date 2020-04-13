#ifndef EU_TGCM_KONTROLLER_PLAYLISTSERVICE_H
#define EU_TGCM_KONTROLLER_PLAYLISTSERVICE_H

#include "playlistitem.h"

#include <QObject>
#include <QVariantList>
#include <QVector>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class Client;
class Player;
/**
 * @brief The PlaylistService class handles the current playlist related operations
 */
class PlaylistService : public QObject
{
	Q_OBJECT

	eu::tgcm::kontroller::Client* client_ = nullptr;
	eu::tgcm::kontroller::Player* player_ = nullptr;
	QVector<PlaylistItem> currentItems_;
	int playlistId_;
	QString playlistType_;
	int playlistPosition_;

	bool refreshing_ = false;
	bool restartAfterRefreshing_ = false;

	int pendingPosition_ = -1;

public:
	explicit PlaylistService(Client* client, eu::tgcm::kontroller::Player* player, QObject *parent = nullptr);

	Q_PROPERTY(int playlistId READ playlistId WRITE setPlaylistId NOTIFY playlistIdChanged)
	Q_PROPERTY(QString playlistType READ playlistType WRITE setPlaylistType NOTIFY playlistTypeChanged)
	Q_PROPERTY(int playlistPosition READ playlistPosition WRITE setPlaylistPosition NOTIFY playlistPositionChanged)
	Q_PROPERTY(QVariantList items READ items NOTIFY itemsChanged)

	int playlistId() const;
	void setPlaylistId(int playlistId);

	QString playlistType() const;
	void setPlaylistType(const QString &playlistType);

	int playlistPosition() const;
	void setPlaylistPosition(int playlistPosition);

	const QVector<PlaylistItem> currentItems() const;
	QVariantList items();


signals:
	void playlistIdChanged();
	void itemsChanged();
	void playlistTypeChanged();
	void playlistPositionChanged();

public slots:
	void switchToItem(int position);
	void clearPlaylist();
	void removeElement(int index);
	/**
	 * @brief refresh refreshes the default playlist from the server
	 */
	void refresh();

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
