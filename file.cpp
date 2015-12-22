#include "file.h"

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

void File::setThumbnail(const QString &thumbnail)
{
    thumbnail_ = thumbnail;
    emit thumbnailChanged();
}

File::File(QObject *parent) :
    QObject(parent)
{
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
    emit fileChanged(file_);
}

void File::setFiletype(QString filetype)
{
    filetype_ = filetype;
    emit filetypeChanged(filetype_);
}

void File::setLabel(QString label)
{
    label_ = label;
    emit labelChanged(label_);
}

void File::setType(QString type)
{
    type_ = type;
    emit typeChanged(type_);
}

}

}
}
