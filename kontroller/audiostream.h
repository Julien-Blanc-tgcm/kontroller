#ifndef EU_TGCM_KONTROLLER_AUDIOSTREAM_H
#define EU_TGCM_KONTROLLER_AUDIOSTREAM_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class AudioStream : public QObject
{
	Q_OBJECT

	int index_ = 0;
	QString name_;
	QString language_;

  public:
	explicit AudioStream(QObject* parent = nullptr);
	Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
	Q_PROPERTY(QString displayName READ displayName NOTIFY nameChanged)
	int index() const;

	QString name() const;

	QString language() const;

	QString displayName() const;

  signals:

	void indexChanged(int index);

	void nameChanged(QString name);

	void languageChanged(QString language);

  public slots:
	void setIndex(int index);
	void setName(QString name);
	void setLanguage(QString language);
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu

#endif // EU_TGCM_KONTROLLER_AUDIOSTREAM_H
