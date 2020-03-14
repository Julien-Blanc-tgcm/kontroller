#ifndef DOWNLOADLOCATION_H
#define DOWNLOADLOCATION_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class DownloadLocation : public QObject
{
	Q_OBJECT
public:
	explicit DownloadLocation(QObject *parent = nullptr);

	enum LocationType {
		Phone = 0,
		MemoryCard = 1
	};

	Q_ENUM(LocationType)

	Q_PROPERTY(int typeAsInt READ typeAsInt WRITE setTypeAsInt NOTIFY typeAsIntChanged)
	Q_PROPERTY(QString baseFolder READ baseFolder WRITE setBaseFolder NOTIFY baseFolderChanged)
	Q_PROPERTY(QString name READ name NOTIFY nameChanged)

	LocationType type() const;

	QString baseFolder() const;

	int typeAsInt() const;

	QString name() const;

signals:

	void typeChanged(LocationType type);

	void typeAsIntChanged(int type);

	void baseFolderChanged(QString baseFolder);

	void nameChanged(QString name);

public slots:
	void setType(LocationType type);
	void setBaseFolder(QString baseFolder);
	void setTypeAsInt(int type);

private:
	LocationType type_;

	QString folder_;
	QString name_;
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu

#endif // DOWNLOADLOCATION_H
