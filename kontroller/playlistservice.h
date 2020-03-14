#ifndef EU_TGCM_KONTROLLER_PLAYLISTSERVICE_H
#define EU_TGCM_KONTROLLER_PLAYLISTSERVICE_H

#include <QObject>
#include "playlistitem.h"
#include <QQmlListProperty>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

/**
 * @brief The PlaylistService class handles the current playlist related operations
 */
class PlaylistService : public QObject
{
    Q_OBJECT

    QList<PlaylistItem*> currentItems_;
    int playlistId_;
    QString playlistType_;
    int playlistPosition_;

    bool refreshing_;
    bool restartAfterRefreshing_;
    explicit PlaylistService(QObject *parent = 0);
public:
    static PlaylistService& instance();

    Q_PROPERTY(int playlistId READ playlistId WRITE setPlaylistId NOTIFY playlistIdChanged)
    Q_PROPERTY(QString playlistType READ playlistType WRITE setPlaylistType NOTIFY playlistTypeChanged)
    Q_PROPERTY(int playlistPosition READ playlistPosition WRITE setPlaylistPosition NOTIFY playlistPositionChanged)

    int playlistId() const;
    void setPlaylistId(int playlistId);

    QString playlistType() const;
    void setPlaylistType(const QString &playlistType);

    int playlistPosition() const;
    void setPlaylistPosition(int playlistPosition);

    QList<PlaylistItem*> const currentItems() const;
    QQmlListProperty<PlaylistItem> items();

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
};

}
}
}

#endif // EU_TGCM_KONTROLLER_PLAYLISTSERVICE_H
