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

KodiPlayer::KodiPlayer(QObject *parent) :
    QObject(parent),
    speed_(0)
{
}
