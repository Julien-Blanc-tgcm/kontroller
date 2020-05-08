#ifndef EU_TGCM_KONTROLLER_PLAYERSERVICE_H
#define EU_TGCM_KONTROLLER_PLAYERSERVICE_H

#include "client.h"
#include "player.h"

#include <QObject>
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
public:
	explicit PlayerService(eu::tgcm::kontroller::Client* client, QObject *parent = nullptr);

	Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::Player> players READ players NOTIFY playersChanged)

private:
	QList<Player*> currentPlayers_;
	QTimer refreshTimer_;
	bool refreshPending_ = false;
	// timer use to delay a refresh, because otherwise kodi information
	// is not accurate
	QTimer playerRefreshTimer_;

	eu::tgcm::kontroller::Client* client_ = nullptr;

signals:
	void playersChanged();

public slots:
	void refreshPlayerInfo();
	void playPause(int playerid);

public:
	QList<Player*> const& playersList();

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
	Q_INVOKABLE Player* getPlayer(QString type);

	QQmlListProperty<Player> players();

private slots:
	void refreshPlayerInfoCb_();


	/**
	 * @brief updateConnectionStatus is called when the connection status changes
	 * basically launches the refresh of player info when connected
	 */
	void updateConnectionStatus_(int);

	void updatePlayerSpeed(int playerId, int speed);
	void stopPlayer_();

	void updatePlayerSeek_(int playerId, int hours, int minutes, int seconds, int milliseconds);
  private:
	static int playerCount_(QQmlListProperty<Player>* list);
	static Player* playerAt_(QQmlListProperty<Player>* list, int index);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_PLAYERSERVICE_H
