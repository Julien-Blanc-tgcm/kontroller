#ifndef EU_TGCM_KONTROLLER_MUSICSERVICE_H
#define EU_TGCM_KONTROLLER_MUSICSERVICE_H

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

class MusicService : public QObject
{
    Q_OBJECT
private:
    QList<File*> files_;

    QString browsingMode_;
    QString browsingValue_;
    QString label_;
    bool refreshing_;
    int audioPlaylistId_;

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
    MusicService(QObject * parent = NULL);
    MusicService(QString browsingMode, QString browsingValue, QObject* parent = NULL);
    ~MusicService();
    QList<File *> files() const;
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
    void setFiles(const QList<eu::tgcm::kontroller::File *> &value);
    void setBrowsingMode(QString browsingMode);
    void setBrowsingValue(QString browsingValue);
    void setLabel(QString label);
    void setRefreshing(bool refreshing);
    void setInputRequested(bool inputRequested);

    void setInputTitle(QString inputTitle);

    void setInputValue(QString inputValue);

    void inputBack();

    void inputText(QString inputValue);

private:

    void clearFiles();
    void refresh_files();
    void refresh_collection();
    void refreshAddons_();

    bool inputRequested_ = false;

    QString inputTitle_;

    QString inputValue_;

private slots:
    void parseArtistsResults();
    void parseAlbumsResults();
    void parseSongsResults();
    void parseGenresResults();
    void parseDirectoryResults();
    void parseRefreshAddonsResult_();
    void requestInput_(QString title, QString type, QString value);
    void requestInputFinished_();
};

}
}
}
#endif // EU_TGCM_KONTROLLER_	MUSICSERVICE_H
