#include "audiostream.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

AudioStream::AudioStream(QObject *parent) : QObject(parent)
{

}

int AudioStream::index() const
{
	return index_;
}

QString AudioStream::name() const
{
	return name_;
}

QString AudioStream::language() const
{
	return language_;
}

QString AudioStream::displayName() const
{
	return name_ + "(" + language_ + ")";
}

void AudioStream::setIndex(int index)
{
	if (index_ == index)
		return;

	index_ = index;
	emit indexChanged(index);
}

void AudioStream::setName(QString name)
{
	if (name_ == name)
		return;

	name_ = name;
	emit nameChanged(name);
}

void AudioStream::setLanguage(QString language)
{
	if (language_ == language)
		return;

	language_ = language;
	emit languageChanged(language);
}

}
}
}
