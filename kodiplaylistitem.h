#ifndef KODIPLAYLISTITEM_H
#define KODIPLAYLISTITEM_H

#include <QObject>

class KodiPlaylistItem : public QObject
{
    Q_OBJECT
    /**
     * @see type()
     */
    QString type_;
    QString file_;
    QString label_;
    int artistId_;
    int albumId_;

public:
    explicit KodiPlaylistItem(QObject *parent = 0);

    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
    Q_PROPERTY(int albumId READ albumId WRITE setAlbumId NOTIFY albumIdChanged)

    /**
     * @brief type the type of the playlist list item, may be photo, video or audio
     * @return
     */
    QString type() const;
    void setType(const QString &type);

    QString file() const;
    void setFile(const QString &file);

    QString label() const;
    void setLabel(const QString &label);

    int artistId() const;
    void setArtistId(int artistId);

    int albumId() const;
    void setAlbumId(int albumId);

signals:
    void typeChanged();
    void fileChanged();
    void labelChanged();
    void artistIdChanged();
    void albumIdChanged();
public slots:
};

#endif // KODIPLAYLISTITEM_H
