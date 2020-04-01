#include "volumeplugin.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{
VolumePlugin::VolumePlugin(QObject *parent) : QObject(parent)
{

}

QString VolumePlugin::name() const
{
	return realName_();
}

int VolumePlugin::maxVolume() const
{
	return maxVolume_();
}

int VolumePlugin::minVolume() const
{
	return minVolume_();
}

int VolumePlugin::currentVolume() const
{
	return currentVolume_();
}

void VolumePlugin::updateVolume(int newVolume)
{
	updateVolume_(newVolume);
}

int VolumePlugin::volumeStep() const
{
	return volumeStep_();
}

int VolumePlugin::status() const
{
	return status_;
}

void VolumePlugin::refreshVolume()
{
	refreshVolume_();
}

void VolumePlugin::increaseVolume()
{
	increaseVolume_();
}

void VolumePlugin::decreaseVolume()
{
	decreaseVolume_();
}

void VolumePlugin::setStatus(int status)
{
	if (status_ == status)
		return;

	status_ = status;
	emit statusChanged(status_);
}

void VolumePlugin::increaseVolume_()
{
	auto vol = std::min(currentVolume_() + volumeStep_(), maxVolume_());
	if(vol != currentVolume_())
		updateVolume_(vol);
}

void VolumePlugin::decreaseVolume_()
{
	auto vol = std::max(currentVolume_() - volumeStep_(), minVolume_());
	if(vol != currentVolume_())
		updateVolume_(vol);
}

}
}
}

