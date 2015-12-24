#include "player.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

int Player::playerId() const
{
    return playerId_;
}

void Player::setPlayerId(int playerId)
{
    playerId_ = playerId;
    emit playerIdChanged();
}

QString Player::type() const
{
    return type_;
}

void Player::setType(const QString &type)
{
    type_ = type;
    emit typeChanged();
}

QString Player::current() const
{
    return current_;
}

void Player::setCurrent(const QString &current)
{
    current_ = current;
    emit currentChanged();
}

int Player::speed() const
{
    return speed_;
}

void Player::setSpeed(int speed)
{
    speed_ = speed;
    emit speedChanged();
    if(speed == 0)
        timer_.stop();
    else
        timer_.start();
}

int Player::playlistPosition() const
{
    return playlistPosition_;
}

void Player::setPlaylistPosition(int playlistPosition)
{
    playlistPosition_ = playlistPosition;
    emit playlistPositionChanged();
}

double Player::percentage() const
{
    return percentage_;
}

int Player::time() const
{
    return time_;
}

int Player::totalTime() const
{
    return totalTime_;
}

bool Player::shuffled() const
{
    return shuffled_;
}

bool Player::canMove() const
{
    return canMove_;
}

bool Player::canRepeat() const
{
    return canRepeat_;
}

bool Player::canShuffle() const
{
    return canShuffle_;
}

int Player::repeat() const
{
    return repeat_;
}

bool Player::live() const
{
    return live_;
}

bool Player::partyMode() const
{
    return partyMode_;
}

namespace {
int propSubtitlesCount(QQmlListProperty<Subtitle>* list)
{
    return static_cast<std::vector<Subtitle*>*>(list->data)->size();
}

Subtitle* propSubtitlesAt(QQmlListProperty<Subtitle>*list, int index)
{
    auto l = static_cast<std::vector<Subtitle*>*>(list->data);
    if(index < (int)l->size())
        return (*l)[index];
    return nullptr;
}
}

QQmlListProperty<Subtitle> Player::subtitles()
{
    return QQmlListProperty<Subtitle>(this, &subtitles_, &propSubtitlesCount, &propSubtitlesAt);
}

void Player::setSubtitles(std::vector<Subtitle*>&& subtitles, int currentSubtitleIndex)
{
    subtitles_ = std::move(subtitles);
    currentSubtitleIndex_ = currentSubtitleIndex;
    emit subtitlesChanged();
    emit currentSubtitleIndexChanged();
}

void Player::setPercentage(double percentage)
{
    if (percentage_ == percentage)
        return;

    percentage_ = percentage;
    emit percentageChanged(percentage);
}

void Player::setTime(int time)
{
    if (time_ == time)
        return;

    time_ = time;
    emit timeChanged(time);
}

void Player::setTotalTime(int totalTime)
{
    if (totalTime_ == totalTime)
        return;

    totalTime_ = totalTime;
    emit totalTimeChanged(totalTime);
    if(totalTime != 0)
        timer_.start();
    else
        timer_.stop();
}

void Player::setShuffled(bool shuffled)
{
    if (shuffled_ == shuffled)
        return;

    shuffled_ = shuffled;
    emit shuffledChanged(shuffled);
}

void Player::setCanMove(bool canMove)
{
    if (canMove_ == canMove)
        return;

    canMove_ = canMove;
    emit canMoveChanged(canMove);
}

void Player::setCanRepeat(bool canRepeat)
{
    if (canRepeat_ == canRepeat)
        return;

    canRepeat_ = canRepeat;
    emit canRepeatChanged(canRepeat);
}

void Player::setCanShuffle(bool canShuffle)
{
    if (canShuffle_ == canShuffle)
        return;

    canShuffle_ = canShuffle;
    emit canShuffleChanged(canShuffle);
}

void Player::setRepeat(int repeat)
{
    if (repeat_ == repeat)
        return;

    repeat_ = repeat;
    emit repeatChanged(repeat);
}

void Player::setLive(bool live)
{
    if (live_ == live)
        return;

    live_ = live;
    emit liveChanged(live);
}

void Player::setPartyMode(bool partyMode)
{
    if (partyMode_ == partyMode)
        return;

    partyMode_ = partyMode;
    emit partyModeChanged(partyMode);
}

void Player::updateTimer_()
{
    time_ += timer_.interval();
    if(totalTime_ != 0)
        setPercentage(100 * (double)time_ / (double)totalTime_);
    emit timeChanged(time_);
}

Player::Player(QObject *parent) :
    QObject(parent),
    speed_(0)
{
    timer_.setInterval(1000);
    connect(&timer_, &QTimer::timeout, this, &Player::updateTimer_);
}

namespace {
int propAudioStreamsCount(QQmlListProperty<AudioStream>* list)
{
    return static_cast<std::vector<AudioStream*>*>(list->data)->size();
}

AudioStream* propAudioStreamsAt(QQmlListProperty<AudioStream>*list, int index)
{
    auto l = static_cast<std::vector<AudioStream*>*>(list->data);
    if(index < (int)l->size())
        return (*l)[index];
    return nullptr;
}
}

QQmlListProperty<eu::tgcm::kontroller::AudioStream> eu::tgcm::kontroller::Player::audioStreams()
{
    return QQmlListProperty<AudioStream>(this, &audioStreams_, &propAudioStreamsCount, &propAudioStreamsAt);
}

void Player::setAudioStreams(std::vector<AudioStream*>&& audioStreams, int currentAudioStreamIndex)
{
    audioStreams_ = std::move(audioStreams);
    currentAudioStreamIndex_ = currentAudioStreamIndex;
    emit audioStreamsChanged();
    emit currentAudioStreamIndexChanged();
}

}
}
}
