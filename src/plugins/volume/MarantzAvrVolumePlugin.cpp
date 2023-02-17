#include "MarantzAvrVolumePlugin.hpp"

#include "../../client.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

MarantzAvrVolumePlugin::MarantzAvrVolumePlugin(Client* parent) : VolumePlugin(parent)
{
	kodiVolumePlugin_ = new KodiVolumePlugin(parent);
	device_ = new avrremote::AvrDevice(this);
	connect(device_, &avrremote::AvrDevice::connectionStatusChanged, this, &MarantzAvrVolumePlugin::handleConnected_);
	connect(device_, &avrremote::AvrDevice::volumeChanged, this, &MarantzAvrVolumePlugin::currentVolumeChanged);
	connect(device_, &avrremote::AvrDevice::currentSourceChanged, this, &MarantzAvrVolumePlugin::currentSourceChanged);
	connect(parent, &Client::playerSpeedChanged, this, &MarantzAvrVolumePlugin::updateAtStartup_);
}

QString MarantzAvrVolumePlugin::static_name()
{
	return QString::fromUtf8("MarantzAvr");
}

QString MarantzAvrVolumePlugin::currentSource() const
{
	return device_->currentSource().value();
}

QString MarantzAvrVolumePlugin::source() const
{
	return source_;
}

void MarantzAvrVolumePlugin::setSource(QString source)
{
	if (source_ == source)
		return;
	source_ = source;
	emit sourceChanged();
}

bool MarantzAvrVolumePlugin::updateSource() const
{
	return updateSource_;
}

void MarantzAvrVolumePlugin::setUpdateSource(bool update)
{
	if (updateSource_ == update)
		return;
	updateSource_ = update;
	emit updateSourceChanged();
}

int MarantzAvrVolumePlugin::kodiForcedVolume() const
{
	return kodiVolume_;
}

void MarantzAvrVolumePlugin::setKodiForcedVolume(int volume)
{
	if (kodiVolume_ == volume)
		return;
	kodiVolume_ = volume;
	emit kodiForcedVolumeChanged();
}

bool MarantzAvrVolumePlugin::forceKodiVolume() const
{
	return forceKodiVolume_;
}

void MarantzAvrVolumePlugin::setForceKodiVolume(bool force)
{
	if (forceKodiVolume_ == force)
		return;
	forceKodiVolume_ = force;
	emit forceKodiVolumeChanged();
}

QString MarantzAvrVolumePlugin::ipAddress() const
{
	return ipAddress_;
}

bool MarantzAvrVolumePlugin::connected() const
{
	return connected_;
}

void MarantzAvrVolumePlugin::setIpAddress(QString ipaddress)
{
	if (ipAddress_ == ipaddress)
		return;

	ipAddress_ = ipaddress;
	emit ipAddressChanged(ipAddress_);
}

void MarantzAvrVolumePlugin::setConnected(bool connected)
{
	if (connected_ == connected)
		return;

	connected_ = connected;
	emit connectedChanged(connected_);
}

QString MarantzAvrVolumePlugin::realName_() const
{
	return static_name();
}

int MarantzAvrVolumePlugin::maxVolume_() const
{
	return 900;
}

int MarantzAvrVolumePlugin::minVolume_() const
{
	return 0;
}

int MarantzAvrVolumePlugin::currentVolume_() const
{
	return device_->volume().value();
}

void MarantzAvrVolumePlugin::updateVolume_(int newVolume)
{
	device_->setVolume(newVolume);
}

int MarantzAvrVolumePlugin::volumeStep_() const
{
	return 5;
}

void MarantzAvrVolumePlugin::refreshVolume_()
{
	device_->refreshVolume();
}

bool MarantzAvrVolumePlugin::valueValid_() const
{
	return connected_;
}

QString MarantzAvrVolumePlugin::displayValue_() const
{
	if (valueValid_())
	{
		return formatVolume_(device_->volume().value());
	}
	return tr("Disconnected");
}

QString MarantzAvrVolumePlugin::formatVolume_(int value) const
{
	return QString::number(value / 10) + "." + QString::number(value % 10);
}

void MarantzAvrVolumePlugin::setMuted_(bool muted)
{
	device_->setMuted(muted);
}

bool MarantzAvrVolumePlugin::muted_() const
{
	return device_->muted();
}

void MarantzAvrVolumePlugin::handleConnected_()
{
	auto status = device_->connectionStatus();
	switch (status)
	{
		case avrremote::AvrDevice::Unconnected:
		case avrremote::AvrDevice::Connecting:
			setConnected(false);
			break;
		case avrremote::AvrDevice::Connected:
			setConnected(true);
			refreshVolume_();
			break;
	}
}

void MarantzAvrVolumePlugin::updateAtStartup_(int /*playerId*/, int speed)
{
	if (speed == 0) // do nothing for stop
		return;
	if (updateSource_)
	{
		if (!device_->mainZoneOn())
			device_->setMainZoneOn(true);
		if (device_->currentSource().value() != source_)
		{
			auto sources = device_->sources();
			for (int i = 0; i < sources.size(); ++i)
			{
				if (source_ == sources[i]) // that's the requested source
					device_->setSource(i);
			}
		}
	}
	if (forceKodiVolume_)
	{
		if (kodiVolumePlugin_->currentVolume() != kodiVolume_)
			kodiVolumePlugin_->updateVolume(kodiVolume_);
	}
}

void MarantzAvrVolumePlugin::setParameters(QVariantMap const& parameters)
{
	setUpdateSource(parameters.value("updateSource").toBool());
	setForceKodiVolume(parameters.value("forceKodiVolume").toBool());
	setKodiForcedVolume(parameters.value("kodiForcedVolume").toInt());
	setIpAddress(parameters.value("address").toString());
	setSource(parameters.value("source").toString());

	if (!ipAddress_.isEmpty())
	{
		device_->setAddress(ipAddress_);
		device_->connectToDevice();
	}
}

const QStringList &MarantzAvrVolumePlugin::availableSources() const
{
	return device_->sources();
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu

