#include "kodifile.h"

QString KodiFile::thumbnail() const
{
    return thumbnail_;
}

void KodiFile::setThumbnail(const QString &thumbnail)
{
    thumbnail_ = thumbnail;
    emit thumbnailChanged();
}

KodiFile::KodiFile(QObject *parent) :
    QObject(parent)
{
}

QString KodiFile::file() const
{
    return file_;
}

QString KodiFile::filetype() const
{
    return filetype_;
}

QString KodiFile::label() const
{
    return label_;
}

QString KodiFile::type() const
{
    return type_;
}

void KodiFile::setFile(QString file)
{
    file_ = file;
    emit fileChanged(file_);
}

void KodiFile::setFiletype(QString filetype)
{
    filetype_ = filetype;
    emit filetypeChanged(filetype_);
}

void KodiFile::setLabel(QString label)
{
    label_ = label;
    emit labelChanged(label_);
}

void KodiFile::setType(QString type)
{
    type_ = type;
    emit typeChanged(type_);
}
