#include "downloadlocation.h"

#include <QFileInfo>

namespace eu
{
namespace tgcm
{

namespace kontroller
{

DownloadLocation::DownloadLocation(QObject *parent) : QObject(parent)
{

}

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
	emit typeChanged(type_);
	emit typeAsIntChanged(typeAsInt());
}

void DownloadLocation::setBaseFolder(QString folder)
{
	if (folder_ == folder)
		return;

	folder_ = folder;
	QFileInfo f{folder};
	name_ = f.baseName();
	emit baseFolderChanged(folder_);
	emit nameChanged(name_);
}

void DownloadLocation::setTypeAsInt(int type)
{
	setType(static_cast<LocationType>(type));
}

}
}
}
