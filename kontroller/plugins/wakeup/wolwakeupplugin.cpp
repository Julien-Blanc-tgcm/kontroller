#include "wolwakeupplugin.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

WolWakeUpPlugin::WolWakeUpPlugin(QObject* parent) : WakeUpPlugin{parent}
{
	socket_ = new QUdpSocket(this);
}

QString WolWakeUpPlugin::name() const
{
	return pluginName();
}

QString WolWakeUpPlugin::macAddress() const
{
	return macAddress_;
}

int WolWakeUpPlugin::port() const
{
	return port_;
}

QString WolWakeUpPlugin::destinationAddress() const
{
	return destinationAddress_;
}

QString WolWakeUpPlugin::pluginName()
{
	return QString::fromUtf8("WolWakeUp");
}

int WolWakeUpPlugin::defaultPort()
{
	return 9;
}

void WolWakeUpPlugin::setMacAddress(QString macAddress)
{
	if (macAddress_ == macAddress)
		return;

	macAddress_ = macAddress;
	emit macAddressChanged(macAddress_);
}

void WolWakeUpPlugin::setPort(int port)
{
	if (port_ == port)
		return;

	port_ = port;
	emit portChanged(port_);
}

void WolWakeUpPlugin::setDestinationAddress(QString destinationAddress)
{
	if (destinationAddress_ == destinationAddress)
		return;

	destinationAddress_ = destinationAddress;
	emit destinationAddressChanged(destinationAddress_);
}
namespace
{
/**
 * @brief getByte_ returns the value of a byte from a mac address in the form AA:BB:CC:DD:EE:FF
 * @param macAddress
 * @param byteIndex
 * @return
 */
quint8 getByte_(QString macAddress, int byteIndex)
{
	auto str = macAddress.mid(byteIndex * 3, 2);
	if (str.isNull())
		return 0xFF;
	bool ok = false;
	quint8 res = static_cast<quint8>(str.toInt(&ok, 16));
	if (!ok)
		return 0xFF;
	return res;
}

} // namespace
void WolWakeUpPlugin::wakeUp_()
{
	QHostAddress dAddress = QHostAddress::Broadcast;
	if (destinationAddress_.size() != 0)
	{
		dAddress = QHostAddress{destinationAddress_};
	}
	QByteArray data;
	data.reserve((1 + 16) * 6);
	for (int i = 0; i < 6; ++i)
		data.push_back(0xFF);
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			data.push_back(getByte_(macAddress_, j));
		}
	}
	socket_->writeDatagram(data, data.size(), dAddress, static_cast<quint16>(port_));
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
