#ifndef PLAYLISTSERVICE_H
#define PLAYLISTSERVICE_H

#include <QObject>
#include <QQmlListProperty>
#include "kodiplaylistitem.h"

/**
 * @brief The PlaylistService class handles the current playlist related operations
 */
class PlaylistService : public QObject
{
    Q_OBJECT

    QList<KodiPlaylistItem*> currentItems_;
    int playlistId_;
    QString playlistType_;
    int playlistPosition_;
public:
    explicit PlaylistService(QObject *parent = 0);

    Q_PROPERTY(int playlistId READ playlistId WRITE setPlaylistId NOTIFY playlistIdChanged)
    Q_PROPERTY(QQmlListProperty<KodiPlaylistItem> items READ items NOTIFY itemsChanged)
    Q_PROPERTY(QString playlistType READ playlistType WRITE setPlaylistType NOTIFY playlistTypeChanged)
    Q_PROPERTY(int playlistPosition READ playlistPosition WRITE setPlaylistPosition NOTIFY playlistPositionChanged)

    int playlistId() const;
    void setPlaylistId(int playlistId);

    QQmlListProperty<KodiPlaylistItem> items();

    QString playlistType() const;
    void setPlaylistType(const QString &playlistType);

    int playlistPosition() const;
    void setPlaylistPosition(int playlistPosition);

signals:
    void playlistIdChanged();
    void itemsChanged();
    void playlistTypeChanged();
    void playlistPositionChanged();
public slots:
    void switchToItem(int position);
private slots:
    void refreshPlaylist_();
    void refreshPlaylistCb_();
    void findMatchingPlaylist_();
    void findMatchingPlaylistCb_();
};

#endif // PLAYLISTSERVICE_H
