#ifndef DOWNLOADLOCATION_H
#define DOWNLOADLOCATION_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class DownloadLocation
{
	Q_GADGET
public:
	DownloadLocation() = default;
	~DownloadLocation() = default;
	DownloadLocation(DownloadLocation const&) = default;
	DownloadLocation(DownloadLocation &&) = default;
	DownloadLocation& operator=(DownloadLocation const&) = default;
	DownloadLocation& operator=(DownloadLocation&&) = default;

	enum LocationType {
		Phone = 0,
		MemoryCard = 1
	};

	Q_ENUM(LocationType)

	Q_PROPERTY(int typeAsInt READ typeAsInt WRITE setTypeAsInt)
	Q_PROPERTY(QString baseFolder READ baseFolder WRITE setBaseFolder)
	Q_PROPERTY(QString name READ name)

	LocationType type() const;

	QString baseFolder() const;

	int typeAsInt() const;

	QString name() const;

	void setType(LocationType type);
	void setBaseFolder(QString baseFolder);
	void setTypeAsInt(int type);

private:
	LocationType type_;

	QString folder_;
	QString name_;
	friend QDataStream& from_stream(QDataStream& stream, DownloadLocation& location);
};

bool operator==(DownloadLocation const& first, DownloadLocation const& second);
bool operator!=(DownloadLocation const& first, DownloadLocation const& second);

QDataStream& from_stream(QDataStream& stream, DownloadLocation& location);

} // namespace kontroller
} // namespace tgcm
} // namespace eu

QDataStream& operator>>(QDataStream& stream, eu::tgcm::kontroller::DownloadLocation& location);
QDataStream& operator<<(QDataStream& stream, eu::tgcm::kontroller::DownloadLocation const& location);

Q_DECLARE_METATYPE(eu::tgcm::kontroller::DownloadLocation)

#endif // DOWNLOADLOCATION_H
