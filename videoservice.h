#ifndef VIDEOSERVICE_H
#define VIDEOSERVICE_H

#include <QObject>
#include <QtQml>
#include "kodifile.h"
#include <QQmlListProperty>

class VideoService : public QObject
{
    Q_OBJECT
private:
    QList<KodiFile*> files_;

    QString browsingMode_;
    QString browsingValue_;
    QString label_;
    bool refreshing_;
    int videoPlaylistId_;

public:
    Q_PROPERTY(QQmlListProperty<KodiFile> filesAsList READ filesAsList NOTIFY filesAsListChanged)
    Q_PROPERTY(QString browsingMode READ browsingMode WRITE setBrowsingMode NOTIFY browsingModeChanged)
    Q_PROPERTY(QString browsingValue READ browsingValue WRITE setBrowsingValue NOTIFY browsingValueChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged)

public:
    VideoService(QObject * parent = NULL);
    VideoService(QString browsingMode, QString browsingValue, QObject* parent = NULL);
    ~VideoService();
    QList<KodiFile *> files() const;
    QQmlListProperty<KodiFile> filesAsList();
    QString browsingMode() const;
    QString browsingValue() const;
    QString label() const;
    bool refreshing() const;

signals:
    void filesAsListChanged();
    void browsingModeChanged();
    void browsingValueChanged();
    void labelChanged();
    void refreshingChanged();
public slots:
    void refresh();
    void setFiles(const QList<KodiFile *> &value);
    void playFile(QObject* file);
    void setBrowsingMode(QString browsingMode);
    void setBrowsingValue(QString browsingValue);
    void setLabel(QString label);
    void setRefreshing(bool refreshing);
    void refreshCollection();
private:
    bool clearPlayList();
    bool addFileToPlaylist(KodiFile* file);
    bool startPlaying();

    void clearFiles();
    void refresh_files();
    void refresh_collection();

private slots:
    void parseMoviesResults_();
    void parseTVShowsResults_();
    void parseMusicVideosResults_();
    void parseGenresResults_();
    void parseSeasonsResults_();
    void parseEpisodesResults_();
    void parseDirectoryResults();
};

#endif // VIDEOSERVICE_H
