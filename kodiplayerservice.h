#ifndef KODIPLAYERSERVICE_H
#define KODIPLAYERSERVICE_H

#include <QObject>
#include "kodiplayer.h"
#include <QTimer>

class KodiPlayerService : public QObject
{
    Q_OBJECT
    explicit KodiPlayerService(QObject *parent = 0);
public:
    static KodiPlayerService& instance();
private:
    QList<KodiPlayer*> currentPlayers_;
    QTimer refreshTimer_;
    bool refreshPending_;

signals:
    void playersChanged();
public slots:
    void refreshPlayerInfo();
    void playPause(int playerid);
public:
    QList<KodiPlayer*> const& players();
    /**
     * @brief getPlayerId Returns the id of the first found player of given type, -1 otherwise
     * @param type type of the player ("audio", "video" or "photo")
     * @return id of the first found player, -1 otherwise
     */
    int getPlayerId(QString type);
    /**
     * @brief getPlayer Returns the first found player of given type, nullptr otherwise
     * @param type
     * @return
     */
    KodiPlayer* getPlayer(QString type);
private slots:
    void refreshPlayerInfoCb_();

    void refreshAudioPlayerStatus_(int playerId);
    void refreshAudioPlayerInfo_();
    /**
     * @brief updatePlayPause_ updates the speed after a play / pause call
     */
    void updatePlayPause_();

    void refreshVideoPlayerStatus(int playerId);
    void refreshVideoPlayerInfo_(int playerId);

    /**
     * @brief updateConnectionStatus is called when the connection status changes
     * basically launches the refresh of player info when connected
     */
    void updateConnectionStatus(int);

    void updatePlayerSpeed(int playerId, int speed);
};

#endif // KODIPLAYERSERVICE_H
