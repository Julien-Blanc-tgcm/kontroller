#ifndef EU_TGCM_KONTROLLER_PLAYERCONTROL_H
#define EU_TGCM_KONTROLLER_PLAYERCONTROL_H

#include <QObject>
#include <QTimer>
#include <QQmlListProperty>

#include "client.h"
#include "player.h"
#include "playerservice.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class PlayerControl : public QObject
{
	Q_OBJECT
public:
	explicit PlayerControl(QObject *parent = 0);
	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::Player> players READ players NOTIFY playersChanged)
private:
	QList<Player*> players_;

	eu::tgcm::kontroller::Client* client_ = nullptr;

signals:
	void playersChanged();

	void clientChanged(eu::tgcm::kontroller::Client* client);

public:
	QQmlListProperty<Player> players();

	eu::tgcm::kontroller::Client* client() const;

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
	void switchAudioStream(int playerId, int audioStreamIndex);

	void setClient(eu::tgcm::kontroller::Client* client);

private slots:
	void updatePlayers_();
	void handleShuffleResult_(int playerId, bool shuffle);
};

}
}
}
#endif // EU_TGCM_KONTROLLER_PLAYERCONTROL_H
