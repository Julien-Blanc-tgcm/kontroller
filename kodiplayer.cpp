#include "kodiplayer.h"

int KodiPlayer::playerId() const
{
    return playerId_;
}

void KodiPlayer::setPlayerId(int playerId)
{
    playerId_ = playerId;
    emit playerIdChanged();
}

QString KodiPlayer::type() const
{
    return type_;
}

void KodiPlayer::setType(const QString &type)
{
    type_ = type;
    emit typeChanged();
}

QString KodiPlayer::current() const
{
    return current_;
}

void KodiPlayer::setCurrent(const QString &current)
{
    current_ = current;
    emit currentChanged();
}

int KodiPlayer::speed() const
{
    return speed_;
}

void KodiPlayer::setSpeed(int speed)
{
    speed_ = speed;
    emit speedChanged();
    if(speed == 0)
        timer_.stop();
    else
        timer_.start();
}

int KodiPlayer::playlistPosition() const
{
    return playlistPosition_;
}

void KodiPlayer::setPlaylistPosition(int playlistPosition)
{
    playlistPosition_ = playlistPosition;
    emit playlistPositionChanged();
}

double KodiPlayer::percentage() const
{
    return percentage_;
}

int KodiPlayer::time() const
{
    return time_;
}

int KodiPlayer::totalTime() const
{
    return totalTime_;
}

bool KodiPlayer::shuffled() const
{
    return shuffled_;
}

bool KodiPlayer::canMove() const
{
    return canMove_;
}

bool KodiPlayer::canRepeat() const
{
    return canRepeat_;
}

bool KodiPlayer::canShuffle() const
{
    return canShuffle_;
}

int KodiPlayer::repeat() const
{
    return repeat_;
}

bool KodiPlayer::live() const
{
    return live_;
}

bool KodiPlayer::partyMode() const
{
    return partyMode_;
}

bool KodiPlayer::subtitlesEnabled() const
{
    return subtitlesEnabled_;
}

void KodiPlayer::setPercentage(double percentage)
{
    if (percentage_ == percentage)
        return;

    percentage_ = percentage;
    emit percentageChanged(percentage);
}

void KodiPlayer::setTime(int time)
{
    if (time_ == time)
        return;

    time_ = time;
    emit timeChanged(time);
}

void KodiPlayer::setTotalTime(int totalTime)
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

void KodiPlayer::setShuffled(bool shuffled)
{
    if (shuffled_ == shuffled)
        return;

    shuffled_ = shuffled;
    emit shuffledChanged(shuffled);
}

void KodiPlayer::setCanMove(bool canMove)
{
    if (canMove_ == canMove)
        return;

    canMove_ = canMove;
    emit canMoveChanged(canMove);
}

void KodiPlayer::setCanRepeat(bool canRepeat)
{
    if (canRepeat_ == canRepeat)
        return;

    canRepeat_ = canRepeat;
    emit canRepeatChanged(canRepeat);
}

void KodiPlayer::setCanShuffle(bool canShuffle)
{
    if (canShuffle_ == canShuffle)
        return;

    canShuffle_ = canShuffle;
    emit canShuffleChanged(canShuffle);
}

void KodiPlayer::setRepeat(int repeat)
{
    if (repeat_ == repeat)
        return;

    repeat_ = repeat;
    emit repeatChanged(repeat);
}

void KodiPlayer::setLive(bool live)
{
    if (live_ == live)
        return;

    live_ = live;
    emit liveChanged(live);
}

void KodiPlayer::setPartyMode(bool partyMode)
{
    if (partyMode_ == partyMode)
        return;

    partyMode_ = partyMode;
    emit partyModeChanged(partyMode);
}

void KodiPlayer::setSubtitlesEnabled(bool subtitlesEnabled)
{
    if (subtitlesEnabled_ == subtitlesEnabled)
        return;

    subtitlesEnabled_ = subtitlesEnabled;
    emit subtitlesEnabledChanged(subtitlesEnabled);
}

void KodiPlayer::updateTimer_()
{
    time_ += timer_.interval();
    if(totalTime_ != 0)
        setPercentage(100 * (double)time_ / (double)totalTime_);
}

KodiPlayer::KodiPlayer(QObject *parent) :
    QObject(parent),
    speed_(0)
{
    timer_.setInterval(1000);
    connect(&timer_, &QTimer::timeout, this, &KodiPlayer::updateTimer_);
}

