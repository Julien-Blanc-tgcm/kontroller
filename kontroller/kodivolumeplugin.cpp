#include "kodivolumeplugin.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{
KodiVolumePlugin::KodiVolumePlugin(QObject *parent) : VolumePlugin(parent)
{

}

QString KodiVolumePlugin::realName_() const
{
	return QString::fromUtf8("Kodi");
}

int KodiVolumePlugin::maxVolume_() const
{
	return 100;
}

int KodiVolumePlugin::minVolume_() const
{
	return 0;
}

int KodiVolumePlugin::currentVolume_() const
{
	return currentVolumeStored_;
}

void KodiVolumePlugin::updateVolume_(int /*newVolume*/)
{
// todo : implement
}

int KodiVolumePlugin::volumeStep_() const
{
	return 2;
}

void KodiVolumePlugin::refreshVolume_()
{
// todo : implement
}

void KodiVolumePlugin::increaseVolume_()
{
// todo : migrate code from remote
}

void KodiVolumePlugin::decreaseVolume_()
{
// todo : migrate code from remote
}

}
}
}

