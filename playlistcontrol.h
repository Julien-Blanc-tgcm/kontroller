#ifndef PLAYLISTCONTROL_H
#define PLAYLISTCONTROL_H

#include <QObject>
#include <QQmlListProperty>
#include "playlistservice.h"

class PlaylistControl : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistControl(QObject *parent = 0);

    Q_PROPERTY(int playlistId READ playlistId NOTIFY playlistIdChanged)
    Q_PROPERTY(QQmlListProperty<PlaylistItem> items READ items NOTIFY itemsChanged)
    Q_PROPERTY(QString playlistType READ playlistType WRITE setPlaylistType NOTIFY playlistTypeChanged)
    Q_PROPERTY(int playlistPosition READ playlistPosition NOTIFY playlistPositionChanged)

    QQmlListProperty<PlaylistItem> items();

    int playlistId() const;
    QString playlistType() const;
    int playlistPosition() const;

signals:
    void playlistIdChanged();
    void itemsChanged();
    void playlistTypeChanged();
    void playlistPositionChanged();
public slots:
    void switchToItem(int position);
    void clearPlaylist();
    void removeElement(int index);
    void setPlaylistType(QString type);
};

#endif // PLAYLISTCONTROL_H
