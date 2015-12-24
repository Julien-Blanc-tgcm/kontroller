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

    int index_;
    QString name_;
    QString language_;

public:
    explicit AudioStream(QObject *parent = 0);
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY nameChanged)
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

QString displayName() const
{
    return name_ + "(" + language_ + ")";
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


}
}
}

#endif // EU_TGCM_KONTROLLER_AUDIOSTREAM_H
