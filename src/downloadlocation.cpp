#include "downloadlocation.h"

#include <QDataStream>
#include <QFileInfo>

namespace eu
{
namespace tgcm
{

namespace kontroller
{

DownloadLocation::LocationType DownloadLocation::type() const
{
	return type_;
}

QString DownloadLocation::baseFolder() const
{
	return folder_;
}

int DownloadLocation::typeAsInt() const
{
	return static_cast<int>(type());
}

QString DownloadLocation::name() const
{
	return name_;
}

void DownloadLocation::setType(DownloadLocation::LocationType type)
{
	if (type_ == type)
		return;

	type_ = type;
}

void DownloadLocation::setBaseFolder(QString folder)
{
	if (folder_ == folder)
		return;

	folder_ = folder;
	QFileInfo f{folder};
	name_ = f.baseName();
}

void DownloadLocation::setTypeAsInt(int type)
{
	setType(static_cast<LocationType>(type));
}

QDataStream& from_stream(QDataStream& stream, DownloadLocation& location)
{
	int val;
	stream >> val;
	location.setTypeAsInt(val);
	return stream >> location.name_ >> location.folder_;
}

bool operator==(const DownloadLocation& first, const DownloadLocation& second)
{
	return first.type() == second.type() && first.baseFolder() == second.baseFolder();
}

bool operator!=(const DownloadLocation& first, const DownloadLocation& second)
{
	return !(first == second);
}

}
}
}

QDataStream& operator<<(QDataStream& stream, const eu::tgcm::kontroller::DownloadLocation& location)
{
	return stream << location.typeAsInt() << location.name() << location.baseFolder();
}



QDataStream& operator>>(QDataStream& stream, eu::tgcm::kontroller::DownloadLocation& location)
{
	return eu::tgcm::kontroller::from_stream(stream, location);
}
