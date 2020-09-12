#include "kodiservicediscovery.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

KodiServiceDiscovery::KodiServiceDiscovery(QObject* parent) :
    QObject(parent), discovery_(new SBZeroConfServiceDiscovery(this))
{
	connect(discovery_, &SBZeroConfServiceDiscovery::serviceDiscovered, this, &KodiServiceDiscovery::addServer_);
	connect(discovery_,
	        &SBZeroConfServiceDiscovery::discoveryFinished,
	        this,
	        &KodiServiceDiscovery::handleDiscoveryFinished_);
	discovery_->setInterface(findRelevantInterface_());
}

QVariantList KodiServiceDiscovery::servers() const
{
	return servers_;
}

bool KodiServiceDiscovery::discovering() const
{
	return discovering_;
}

void KodiServiceDiscovery::startDiscovery()
{
	discovery_->query("_xbmc-jsonrpc-h._tcp.local.", std::chrono::seconds{10});
	setDiscovering_(true);
}

void KodiServiceDiscovery::abortDiscovery()
{
	discovery_->interruptCurrentOperation();
	setDiscovering_(false);
}

void KodiServiceDiscovery::setDiscovering_(bool discovering)
{
	if (discovering_ == discovering)
		return;

	discovering_ = discovering;
	emit discoveringChanged(discovering_);
}

void KodiServiceDiscovery::handleDiscoveryFinished_()
{
	setDiscovering_(false);
}

void KodiServiceDiscovery::addServer_(SBZeroConfServiceRecord record)
{
	servers_.push_back(QVariant::fromValue(record));
	emit serversChanged(servers_);
}

QNetworkInterface KodiServiceDiscovery::findRelevantInterface_()
{
	for (auto interface : QNetworkInterface::allInterfaces())
	{
		auto flags = interface.flags();
		if (flags & QNetworkInterface::IsLoopBack)
			continue;
		if ((flags & QNetworkInterface::IsUp) == 0)
			continue;
		if ((flags & QNetworkInterface::CanMulticast) == 0)
			continue;
		if (flags & interface.name().startsWith("w"))
		{
			qDebug() << "Using interface " << interface.name();
			return interface;
		}
	}
	qDebug() << "No suitable interface found";
	return QNetworkInterface();
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
