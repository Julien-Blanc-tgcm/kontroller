#ifndef EU_TGCM_KONTROLLER_VIDEOSERVICE_H
#define EU_TGCM_KONTROLLER_VIDEOSERVICE_H

#include <QObject>
#include <QtQml>
#include "file.h"
#include <QQmlListProperty>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class VideoService : public QObject
{
    Q_OBJECT
private:
    std::vector<File*> files_;

    QString browsingMode_;
    QString browsingValue_;
    QString label_;
    bool refreshing_;

    bool inputRequested_;

    QString inputTitle_;

    QString inputValue_;

public:
    Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::File> filesAsList READ filesAsList NOTIFY filesAsListChanged)
    Q_PROPERTY(QString browsingMode READ browsingMode WRITE setBrowsingMode NOTIFY browsingModeChanged)
    Q_PROPERTY(QString browsingValue READ browsingValue WRITE setBrowsingValue NOTIFY browsingValueChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged)

    Q_PROPERTY(bool inputRequested READ inputRequested WRITE setInputRequested NOTIFY inputRequestedChanged)
    Q_PROPERTY(QString inputTitle READ inputTitle WRITE setInputTitle NOTIFY inputTitleChanged)
    Q_PROPERTY(QString inputValue READ inputValue WRITE setInputValue NOTIFY inputValueChanged)

public:
    VideoService(QObject * parent = NULL);
    VideoService(QString browsingMode, QString browsingValue, QObject* parent = NULL);
    ~VideoService();
    std::vector<File *> files() const;
    QQmlListProperty<File> filesAsList();
    QString browsingMode() const;
    QString browsingValue() const;
    QString label() const;
    bool refreshing() const;

    bool inputRequested() const;

    QString inputTitle() const;

    QString inputValue() const;

signals:
    void filesAsListChanged();
    void browsingModeChanged();
    void browsingValueChanged();
    void labelChanged();
    void refreshingChanged();
    void inputRequestedChanged(bool inputRequested);

    void inputTitleChanged(QString inputTitle);

    void inputValueChanged(QString inputValue);

public slots:
    void refresh();
    void setFiles(const std::vector<File *> &value);
    void setBrowsingMode(QString browsingMode);
    void setBrowsingValue(QString browsingValue);
    void setLabel(QString label);
    void setRefreshing(bool refreshing);
    void refreshCollection();



    void setInputRequested(bool inputRequested);

    void setInputTitle(QString inputTitle);

    void setInputValue(QString inputValue);

private:
    bool clearPlayList();
    bool addFileToPlaylist(File* file);
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
    void requestInput_(QString title, QString type, QString value);
    void requestInputFinished_();
};

}
}
}

#endif // EU_TGCM_KONTROLLER_VIDEOSERVICE_H
