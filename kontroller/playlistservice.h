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

	bool refreshing_ = false;
	bool restartAfterRefreshing_ = false;

	int pendingPosition_ = -1;

public:
	explicit PlaylistService(Client* client, eu::tgcm::kontroller::Player* player, QObject *parent = nullptr);

	Q_PROPERTY(int playlistId READ playlistId NOTIFY playlistIdChanged)
	Q_PROPERTY(QString playlistType READ playlistType NOTIFY playlistTypeChanged)
	Q_PROPERTY(int playlistPosition READ playlistPosition NOTIFY playlistPositionChanged)
	Q_PROPERTY(QVariantList items READ items NOTIFY itemsChanged)

	int playlistId() const;

	QString playlistType() const;

	int playlistPosition() const;

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
//	void setCurrentlyPlayedItem_(int playerId, QString type, int id, QString file);
	void refreshPlaylist_();
	void refreshPlaylistCb_();
	void handlePlaylistCleared_(int playlist);
	void handlePlaylistElementRemoved(int playlist, int position);
	void handlePlaylistElementAdded(int playlistId);
	void handleGotoReply_();
};

}
}
}

#endif // EU_TGCM_KONTROLLER_PLAYLISTSERVICE_H
