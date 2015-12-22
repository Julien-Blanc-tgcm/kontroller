#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <QObject>
#include <QTimer>
#include <QQmlListProperty>
#include "kodiplayer.h"

class PlayerControl : public QObject
{
    Q_OBJECT
public:
    explicit PlayerControl(QObject *parent = 0);
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
    void seekBackward(int playerId);
    void seekForward(int playerId);
    void seekToPercentage(int playerId, int percentage);
    void setShuffle(int playerId, bool shuffle);

    void switchSubtitle(int playerId, int subtitleIndex);
private slots:
    void updatePlayers_();
    void handleShuffleResult_(int playerId, bool shuffle);
};

#endif // PLAYERCONTROL_H
