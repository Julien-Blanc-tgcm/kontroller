#include "file.h"

#include <QDataStream>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

QString File::thumbnail() const
{
	return thumbnail_;
}

void File::setThumbnail(QString thumbnail)
{
	thumbnail_ = thumbnail;
}

QString File::icon() const
{
	return icon_;
}

bool File::played() const
{
	return played_;
}

void File::setId(int id)
{
	id_ = id;
}

void File::setIcon(QString icon)
{
	icon_ = icon;
}

void File::setPlayed(bool played)
{
	played_ = played;
}

int File::id() const
{
	return id_;
}

QString File::file() const
{
	return file_;
}

QString File::filetype() const
{
	return filetype_;
}

QString File::label() const
{
	return label_;
}

QString File::type() const
{
	return type_;
}

void File::setFile(QString file)
{
	file_ = file;
}

void File::setFiletype(QString filetype)
{
	filetype_ = filetype;
}

void File::setLabel(QString label)
{
	label_ = label;
}

void File::setType(QString type)
{
	type_ = type;
}

}

}
}

QDataStream& operator>>(QDataStream& stream, eu::tgcm::kontroller::File& file)
{
	QString data;
	int id_;
	stream >> id_;
	file.setId(id_);
	stream >> data;
	file.setFile(data);
	stream >> data;
	file.setFiletype(data);
	stream >> data;
	file.setLabel(data);
	stream >> data;
	file.setType(data);
	stream >> data;
	file.setThumbnail(data);
	stream >> data;
	file.setIcon(data);
	bool p = false;
	stream >> p;
	file.setPlayed(p);
	return stream;
}

QDataStream& operator<<(QDataStream& stream, const eu::tgcm::kontroller::File& file)
{
	return stream << file.id() << file.file() << file.filetype() << file.label() << file.type() << file.thumbnail()
	              << file.icon() << file.played();
}
