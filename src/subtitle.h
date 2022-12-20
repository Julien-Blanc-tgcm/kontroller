#ifndef EU_TGCM_KONTROLLER_SUBTITLE_H
#define EU_TGCM_KONTROLLER_SUBTITLE_H

#include <QObject>
namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Subtitle : public QObject
{
	Q_OBJECT

	int index_ = 0;
	QString name_;
	QString language_;

  public:
	explicit Subtitle(QObject* parent = nullptr);

	Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
	Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)

	int index() const
	{
		return index_;
	}

	QString name() const
	{
		return name_;
	}

	QString language() const
	{
		return language_;
	}

  signals:

	void indexChanged(int index);

	void nameChanged(QString name);

	void languageChanged(QString language);

  public slots:
	void setIndex(int index)
	{
		if (index_ == index)
			return;

		index_ = index;
		emit indexChanged(index);
	}
	void setName(QString name)
	{
		if (name_ == name)
			return;

		name_ = name;
		emit nameChanged(name);
	}
	void setLanguage(QString language)
	{
		if (language_ == language)
			return;

		language_ = language;
		emit languageChanged(language);
	}
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu
#endif // EU_TGCM_KONTROLLER_SUBTITLE_H
