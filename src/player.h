#ifndef EU_TGCM_KONTROLLER_PLAYER_H
#define EU_TGCM_KONTROLLER_PLAYER_H

#include "audiostream.h"
#include "subtitle.h"

#include "playinginformation.h"

#include <QObject>
#include <QQmlListProperty>
#include <QTimer>
#include <QVector>

class QJsonRpcServiceReply;

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Player : public QObject
{
	Q_OBJECT

	int playerId_; // initialized by constructor
	QString type_;
	QString current_;
	int speed_ = 0;
	int playlistPosition_ = -1;
	double percentage_ = 0;
	int totalTime_ = 0;
	int time_ = 0;

	QTimer timer_;

	bool shuffled_ = false;
	bool canMove_ = false;
	bool canRepeat_ = false;
	bool canShuffle_ = false;
	int repeat_ = 0;
	bool live_ = false;
	bool partyMode_ = false;
	bool subtitlesEnabled_ = false;
	bool canSeek_ = false;
	int playlistId_ = -1;
	QVector<Subtitle*> subtitles_;
	int currentSubtitleIndex_ = -1;
	QVector<AudioStream*> audioStreams_;
	int currentAudioStreamIndex_ = -1;
	bool active_ = false;

	Client* client_ = nullptr;
	eu::tgcm::kontroller::PlayingInformation* playingInformation_ = nullptr;
	PlaylistService* playlistService_ = nullptr;

public:
	explicit Player(Client* client, int playerId, QObject *parent = nullptr);

	Q_PROPERTY(int playerId READ playerId WRITE setPlayerId NOTIFY playerIdChanged)
	Q_PROPERTY(QString type READ type NOTIFY typeChanged)
	Q_PROPERTY(QString current READ current NOTIFY currentChanged)
	Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
	Q_PROPERTY(int playlistPosition READ playlistPosition WRITE setPlaylistPosition NOTIFY playlistPositionChanged)
	Q_PROPERTY(double percentage READ percentage WRITE setPercentage NOTIFY percentageChanged)
	Q_PROPERTY(int time READ time NOTIFY timeChanged)
	Q_PROPERTY(int totalTime READ totalTime NOTIFY totalTimeChanged)
	Q_PROPERTY(bool shuffled READ shuffled WRITE setShuffled NOTIFY shuffledChanged)
	Q_PROPERTY(bool canMove READ canMove NOTIFY canMoveChanged)
	Q_PROPERTY(bool canRepeat READ canRepeat NOTIFY canRepeatChanged)
	Q_PROPERTY(bool canShuffle READ canShuffle NOTIFY canShuffleChanged)
	Q_PROPERTY(int repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)
	Q_PROPERTY(bool live READ live NOTIFY liveChanged)
	Q_PROPERTY(bool partyMode READ partyMode NOTIFY partyModeChanged)
	Q_PROPERTY(bool canSeek READ canSeek NOTIFY canSeekChanged)
	Q_PROPERTY(int playlistId READ playlistId NOTIFY playlistIdChanged)
	Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::Subtitle> subtitles READ subtitles NOTIFY subtitlesChanged)
	Q_PROPERTY(int currentSubtitleIndex READ currentSubtitleIndex WRITE setCurrentSubtitleIndex \
	           NOTIFY currentSubtitleIndexChanged)
	Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::AudioStream> audioStreams READ audioStreams NOTIFY audioStreamsChanged)
	Q_PROPERTY(int currentAudioStreamIndex READ currentAudioStreamIndex WRITE setCurrentAudioStreamIndex NOTIFY currentAudioStreamIndexChanged)

	Q_PROPERTY(eu::tgcm::kontroller::PlayingInformation* playingInformation READ playingInformation
	           WRITE setPlayingInformation NOTIFY playingInformationChanged)
	Q_PROPERTY(eu::tgcm::kontroller::PlaylistService* playlistService READ playlistService NOTIFY playlistServiceChanged)
	Q_PROPERTY(bool active READ active NOTIFY activeChanged)

	/*!
	 * \brief playerId id of the player
	 * \return
	 */
	int playerId() const;
	void setPlayerId(int playerId);

	QString type() const;

	// is used by playerservice
	void setType(const QString &type);

	QString current() const;
	void setCurrent(const QString &current);

	int speed() const;
	// is used by player service
	void setSpeed(int speed);

	int playlistPosition() const;
	void setPlaylistPosition(int playlistPosition);

	double percentage() const;

	int time() const;
	// is used by player service to update the time when a notification is received
	void setTime(int time);

	int totalTime() const;

	bool shuffled() const;

	bool canMove() const;

	bool canRepeat() const;

	bool canShuffle() const;

	int repeat() const;

	bool live() const;

	bool partyMode() const;

	bool subtitlesEnabled() const;

	bool canSeek() const;

	int playlistId() const;

	int currentSubtitleIndex() const;

	QQmlListProperty<Subtitle> subtitles();

	QQmlListProperty<AudioStream> audioStreams();

	void setAudioStreams(QVector<AudioStream*> audioStreams, int currentAudioStreamIndex);

	int currentAudioStreamIndex() const;

	eu::tgcm::kontroller::PlayingInformation* playingInformation();
	eu::tgcm::kontroller::PlaylistService* playlistService();

	Q_INVOKABLE void stop();
	Q_INVOKABLE void seekBackward();
	Q_INVOKABLE void seekForward();
	Q_INVOKABLE void playPause();
	Q_INVOKABLE void previous();
	Q_INVOKABLE void next();
	Q_INVOKABLE void moveToFirst();

	/**
	 * Tells that the player is active, ie it is currently playing something
	 */
	bool active() const;

	// can be set from c++, but not from qml
	void setActive(bool value);

	/**
	 * @brief updateProperty updates a single property, as received by a OnPropertyChanged
	 * event from kodi
	 * @param property the property object
	 */
	void updateProperty(QJsonObject const property);

signals:
	void playerIdChanged();
	void typeChanged();
	void currentChanged();
	void speedChanged();
	void playlistPositionChanged();
	void percentageChanged(double percentage);
	void totalTimeChanged(int totalTime);
	void timeChanged(int time);

	void shuffledChanged(bool shuffled);

	void canMoveChanged(bool canMove);

	void canRepeatChanged(bool canRepeat);

	void canShuffleChanged(bool canShuffle);

	void repeatChanged(int repeat);

	void liveChanged(bool live);

	void partyModeChanged(bool partyMode);

	void canSeekChanged(bool canSeek);

	void playlistIdChanged(int playlistId);

	void currentSubtitleIndexChanged();
	void subtitlesChanged();

	void audioStreamsChanged();
	void currentAudioStreamIndexChanged();

	void playingInformationChanged(eu::tgcm::kontroller::PlayingInformation* playingInformation);
	void playlistServiceChanged();

	void activeChanged(bool active);

public slots:
	void setPercentage(double percentage);

	void setShuffled(bool shuffled);

	/**
	 * @brief setRepeat set repeat. 0 is off, 1 is song, 2 is all
	 * @param repeat
	 */
	void setRepeat(int repeat);

	void setCurrentSubtitleIndex(int index);

	void setCurrentAudioStreamIndex(int index);

	void setPlayingInformation(eu::tgcm::kontroller::PlayingInformation* playingInformation);

	void refreshPlayerStatus();

private slots:
	void updateTimer_();

	void handlePlayerStatus_();

private:
	// these functions are the internal ones, used to alter the internal variables, received from the json rpc replies
	void setType_(QString type);
	void setPercentage_(double percentage);
	void setTime_(int time);
	void setTotalTime_(int totalTime);
	void setShuffled_(bool shuffled);
	void setCanMove_(bool canMove);
	void setCanRepeat_(bool canRepeat);
	void setCanShuffle_(bool canShuffle);
	void setRepeat_(int repeat);
	void setLive_(bool live);
	void setPartyMode_(bool partyMode);
	void setSpeed_(int speed);
	void setCanSeek_(bool canSeek);
	void setPlaylistId_(int playlistId);
	void setPlaylistPosition_(int playlistPosition);
	void setCurrentSubtitleIndex_(int index);

	void setCurrentAudioStreamIndex_(int index);

private slots:
	void handleShuffleResult_(QJsonRpcServiceReply* reply, bool shuffle);
	void handleSetAudioStreamResult_(QJsonRpcServiceReply* reply, int index);
	void handleSetSubtitleResult_(QJsonRpcServiceReply* reply, int index);
	void handlePlayPause_();
	void handleSetRepeatResult_(QJsonRpcServiceReply* reply, QString repeat);
	void handleGetItemResponse_();

	void refreshCurrentlyPlaying_();
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu
#endif // EU_TGCM_KONTROLLER_PLAYER_H
