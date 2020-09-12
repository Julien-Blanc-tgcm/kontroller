#ifndef EU_TGCM_KONTROLLER_VIDEOSERVICE_H
#define EU_TGCM_KONTROLLER_VIDEOSERVICE_H

#include "client.h"

#include "file.h"
#include <QObject>
#include <QQmlListProperty>
#include <QtQml>

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
	QVector<File> files_;

	QString browsingMode_;
	QString browsingValue_;
	bool refreshing_;

	bool inputRequested_;

	QString inputTitle_;

	QString inputValue_;

  public:
	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	// Q_PROPERTY(QVector<eu::tgcm::kontroller::File> files READ files NOTIFY filesChanged)
	Q_PROPERTY(QVariantList filesAsList READ filesAsList NOTIFY filesChanged)
	Q_PROPERTY(QString browsingMode READ browsingMode WRITE setBrowsingMode NOTIFY browsingModeChanged)
	Q_PROPERTY(QString browsingValue READ browsingValue WRITE setBrowsingValue NOTIFY browsingValueChanged)
	Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged)

	Q_PROPERTY(bool inputRequested READ inputRequested WRITE setInputRequested NOTIFY inputRequestedChanged)
	Q_PROPERTY(QString inputTitle READ inputTitle WRITE setInputTitle NOTIFY inputTitleChanged)
	Q_PROPERTY(QString inputValue READ inputValue WRITE setInputValue NOTIFY inputValueChanged)

	Q_PROPERTY(bool alwaysDisplayIcons READ alwaysDisplayIcons NOTIFY browsingModeChanged)
  public:
	VideoService(QObject* parent = nullptr);
	~VideoService();
	QVector<File> files() const;

	QVariantList filesAsList() const;

	QString browsingMode() const;
	QString browsingValue() const;
	bool refreshing() const;

	bool inputRequested() const;

	QString inputTitle() const;

	QString inputValue() const;

	eu::tgcm::kontroller::Client* client() const;

	bool alwaysDisplayIcons() const;

  signals:
	void filesChanged();
	void browsingModeChanged();
	void browsingValueChanged();
	void refreshingChanged();
	void inputRequestedChanged(bool inputRequested);

	void inputTitleChanged(QString inputTitle);

	void inputValueChanged(QString inputValue);

	void clientChanged(eu::tgcm::kontroller::Client* client);

  public slots:
	void refresh();
	void setFiles(const QVector<File>& value);
	void setBrowsingMode(QString browsingMode);
	void setBrowsingValue(QString browsingValue);
	void setRefreshing(bool refreshing);
	void refreshCollection();

	void setInputRequested(bool inputRequested);

	void setInputTitle(QString inputTitle);

	void setInputValue(QString inputValue);

	void setClient(eu::tgcm::kontroller::Client* client);

  private:
	bool clearPlayList();
	bool startPlaying();

	void clearFiles();
	void refresh_files();
	void refresh_collection();

	eu::tgcm::kontroller::Client* client_ = nullptr;

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

} // namespace kontroller
} // namespace tgcm
} // namespace eu

#endif // EU_TGCM_KONTROLLER_VIDEOSERVICE_H
