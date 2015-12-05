#ifndef KODIPLAYER_H
#define KODIPLAYER_H

#include <QObject>

class KodiPlayer : public QObject
{
    Q_OBJECT

    int playerId_;
    QString type_;
    QString current_;
    int speed_;
    int playlistPosition_;
public:
    explicit KodiPlayer(QObject *parent = 0);

    Q_PROPERTY(int playerId READ playerId WRITE setPlayerId NOTIFY playerIdChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString current READ current WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int playlistPosition READ playlistPosition WRITE setPlaylistPosition NOTIFY playlistPositionChanged)

    int playerId() const;
    Q_INVOKABLE void setPlayerId(int playerId);

    QString type() const;
    void setType(const QString &type);

    QString current() const;
    void setCurrent(const QString &current);

    int speed() const;
    void setSpeed(int speed);

    int playlistPosition() const;
    void setPlaylistPosition(int playlistPosition);

signals:
    void playerIdChanged();
    void typeChanged();
    void currentChanged();
    void speedChanged();
    void playlistPositionChanged();
public slots:

};

#endif // KODIPLAYER_H
