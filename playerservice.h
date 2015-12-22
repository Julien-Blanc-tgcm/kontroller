#ifndef EU_TGCM_KONTROLLER_PLAYERSERVICE_H
#define EU_TGCM_KONTROLLER_PLAYERSERVICE_H

#include <QObject>
#include "player.h"
#include <QTimer>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class PlayerService : public QObject
{
    Q_OBJECT
    explicit PlayerService(QObject *parent = 0);
public:
    static PlayerService& instance();
private:
    QList<Player*> currentPlayers_;
    QTimer refreshTimer_;
    bool refreshPending_;
    // timer use to delay a refresh, because otherwise kodi information
    // is not accurate
    QTimer playerRefreshTimer_;

signals:
    void playersChanged();
public slots:
    void refreshPlayerInfo();
    void playPause(int playerid);
public:
    QList<Player*> const& players();
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
    Player* getPlayer(QString type);
private slots:
    void refreshPlayerInfoCb_();

    /**
     * @brief updatePlayPause_ updates the speed after a play / pause call
     */
    void updatePlayPause_();

    void refreshPlayerStatus(int playerId);
    void refreshPlayerInfo_(int playerId);

    /**
     * @brief updateConnectionStatus is called when the connection status changes
     * basically launches the refresh of player info when connected
     */
    void updateConnectionStatus(int);

    void updatePlayerSpeed(int playerId, int speed);
    void stopPlayer_();

    void updatePlayerSeek_(int playerId, int hours, int minutes, int seconds, int milliseconds);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_PLAYERSERVICE_H
