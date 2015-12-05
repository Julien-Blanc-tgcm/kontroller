#ifndef KODIPLAYERCONTROL_H
#define KODIPLAYERCONTROL_H

#include <QObject>
#include <QTimer>
#include <QQmlListProperty>
#include "kodiplayer.h"

class KodiPlayerControl : public QObject
{
    Q_OBJECT
public:
    explicit KodiPlayerControl(QObject *parent = 0);
    Q_PROPERTY(QQmlListProperty<KodiPlayer> players READ players NOTIFY playersChanged)
private:
    QList<KodiPlayer*> players_;
signals:
    void playersChanged();
public:
    QQmlListProperty<KodiPlayer> players();

public slots:
    void refreshPlayerInfo();
    void playPause(int playerId);
    void next(int playerId);
    void previous(int playerId);
    void moveToFirst(int playerId);
    void stop(int playerId);
private slots:
    void updatePlayers_();
};

#endif // KODIPLAYERCONTROL_H
