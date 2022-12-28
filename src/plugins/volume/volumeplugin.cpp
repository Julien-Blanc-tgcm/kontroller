#include "volumeplugin.h"

#include "kodivolumeplugin.h"
#include "MarantzAvrVolumePlugin.hpp"
#include "minidspvolumeplugin.h"

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

void VolumePlugin::setMuted(bool muted)
{
	setMuted_(muted);
}

int VolumePlugin::volumeStep() const
{
	return volumeStep_();
}

int VolumePlugin::status() const
{
	return status_;
}

QString VolumePlugin::displayValue() const
{
	return displayValue_();
}

bool VolumePlugin::valueValid() const
{
	return valueValid_();
}

QString VolumePlugin::formatVolume(int volume) const
{
	return formatVolume_(volume);
}

bool VolumePlugin::muted() const
{
	return muted_();
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

VolumePlugin* VolumePlugin::getVolumePlugin(Client* client, QString const& pluginName, QVariantMap const& pluginSettings)
{
	if (pluginName == KodiVolumePlugin::static_name())
		return new KodiVolumePlugin(client);
	if (pluginName == MinidspVolumePlugin::static_name())
	{
		QString address = pluginSettings.value("address").toString();
		auto plugin = new MinidspVolumePlugin(client);
		plugin->setIpAddress(address);
		return plugin;
	}
	if (pluginName == MarantzAvrVolumePlugin::static_name())
	{
		auto plugin = new MarantzAvrVolumePlugin(client);
		plugin->setParameters(pluginSettings);
		return plugin;
	}
	return new KodiVolumePlugin(client); // by default, return a kodi volume plugin. This, at least, is safe
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu

