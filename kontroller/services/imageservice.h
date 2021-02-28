#ifndef EU_TGCM_KONTROLLER_MUSICSERVICE_H
#define EU_TGCM_KONTROLLER_MUSICSERVICE_H

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

class ImageService : public QObject
{
	Q_OBJECT
  private:
	QVector<File> files_;

	QString browsingMode_;
	QString browsingValue_;
	bool refreshing_ = false;

  public:
	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	Q_PROPERTY(QVariantList filesAsList READ filesAsList NOTIFY filesAsListChanged)
	Q_PROPERTY(QString browsingMode READ browsingMode WRITE setBrowsingMode NOTIFY browsingModeChanged)
	Q_PROPERTY(QString browsingValue READ browsingValue WRITE setBrowsingValue NOTIFY browsingValueChanged)
	Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged)

	Q_PROPERTY(bool inputRequested READ inputRequested WRITE setInputRequested NOTIFY inputRequestedChanged)
	Q_PROPERTY(QString inputTitle READ inputTitle WRITE setInputTitle NOTIFY inputTitleChanged)
	Q_PROPERTY(QString inputValue READ inputValue WRITE setInputValue NOTIFY inputValueChanged)
	Q_PROPERTY(bool alwaysDisplayIcons READ alwaysDisplayIcons NOTIFY browsingModeChanged)

  public:
	ImageService(QObject* parent = nullptr);
	ImageService(QString browsingMode, QString browsingValue, QObject* parent = nullptr);
	~ImageService() noexcept override;
	ImageService(ImageService&&) = delete;
	ImageService(ImageService const &) = delete;
	ImageService& operator=(ImageService&&) = delete;
	ImageService& operator=(ImageService const &) = delete;

	QVector<File> files() const;
	QVariantList filesAsList();
	QString browsingMode() const;
	QString browsingValue() const;
	bool refreshing() const;

	bool inputRequested() const;

	QString inputTitle() const;

	QString inputValue() const;

	eu::tgcm::kontroller::Client* client() const;

	bool alwaysDisplayIcons() const;

signals:
	void filesAsListChanged();
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
	void setInputRequested(bool inputRequested);

	void setInputTitle(QString inputTitle);

	void setInputValue(QString inputValue);

	void inputBack();

	void inputText(QString inputValue);

	void setClient(eu::tgcm::kontroller::Client* client);

  private:
	void clearFiles();
	void refresh_files();
	void refreshAddons_();

	bool inputRequested_ = false;

	QString inputTitle_;

	QString inputValue_;

	eu::tgcm::kontroller::Client* client_ = nullptr;

  private slots:
	void parseDirectoryResults();
	void parseRefreshAddonsResult_();
	void requestInput_(QString title, QString type, QString value);
	void requestInputFinished_();
};
} // namespace kontroller
} // namespace tgcm
} // namespace eu
#endif // EU_TGCM_KONTROLLER_	MUSICSERVICE_H
