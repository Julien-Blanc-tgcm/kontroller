#include "minidspvolumeplugin.h"

#include "../../client.h"

#include <QTimer>
#include <QTcpSocket>

#include <minidsplib/query.h>
#include <minidsplib/reply.h>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
MinidspVolumePlugin::MinidspVolumePlugin(Client *parent) :
    VolumePlugin(parent),
    timer_{new QTimer{this}},
    socket_{new QTcpSocket{this}}
{
	connect(socket_, &QTcpSocket::connected, this, &MinidspVolumePlugin::handleConnected_);
	connect(socket_, &QTcpSocket::readyRead, this, &MinidspVolumePlugin::handleReply_);
	connect(socket_, &QTcpSocket::disconnected, this, &MinidspVolumePlugin::handleDisconnected_);
}

QString MinidspVolumePlugin::static_name()
{
	return QString::fromUtf8("Minidsp");
}

QString MinidspVolumePlugin::ipAddress() const
{
	return ipAddress_;
}

bool MinidspVolumePlugin::connected() const
{
	return connected_;
}

void MinidspVolumePlugin::setIpAddress(QString ipaddress)
{
	if (ipAddress_ == ipaddress)
		return;

	ipAddress_ = ipaddress;
	emit ipAddressChanged(ipAddress_);
}

void MinidspVolumePlugin::setConnected(bool connected)
{
	if (connected_ == connected)
		return;

	connected_ = connected;
	emit connectedChanged(connected_);
	if (!pendingQueries_.empty())
		executeNextQuery_();
}

QString MinidspVolumePlugin::realName_() const
{
	return static_name();
}

int MinidspVolumePlugin::maxVolume_() const
{
	return 255;
}

int MinidspVolumePlugin::minVolume_() const
{
	return 0;
}

int MinidspVolumePlugin::currentVolume_() const
{
	return currentVolumeStored_ + 255;
}

void MinidspVolumePlugin::updateVolume_(int newVolume)
{
	auto q = minidsp::Query::changeVolumeQuery(newVolume - 255);
	if(minidsp::Query::isValid(q))
	{
		pushPendingQuery_(q);
		executeNextQuery_();
	}
// todo : implement
}

int MinidspVolumePlugin::volumeStep_() const
{
	return 2;
}

void MinidspVolumePlugin::refreshVolume_()
{
	auto q = minidsp::Query::deviceInformationQuery();
	pushPendingQuery_(q);
	executeNextQuery_();
}

bool MinidspVolumePlugin::valueValid_() const
{
	return connected_ && currentVolumeStored_ != 1;
}

QString MinidspVolumePlugin::displayValue_() const
{
	if (valueValid_())
	{
		return QString::number(static_cast<double>(currentVolumeStored_) / 2) + " dB";
	}
	return "";
}

QString MinidspVolumePlugin::formatVolume_(int value) const
{
	return QString::number(static_cast<double>(value - 255) / 2) + " dB";
}

void MinidspVolumePlugin::setMuted_(bool muted)
{
	auto q = minidsp::Query::muteQuery(muted);
	pushPendingQuery_(q);
	executeNextQuery_();
	Q_UNUSED(muted);
}

bool MinidspVolumePlugin::muted_() const
{
	return mutedStored_;
}

void MinidspVolumePlugin::pushPendingQuery_(const minidsp::Query::Message& query)
{
	pendingQueries_.push_back(query);
}

void MinidspVolumePlugin::executeNextQuery_()
{
	if(!connected_)
	{
		socket_->connectToHost(ipAddress_, 5333);
	}
	else
	{
		auto q = pendingQueries_.front();
		pendingQueries_.erase(pendingQueries_.begin());
		executeQuery_(q);
	}
}

void MinidspVolumePlugin::executeQuery_(minidsp::Query::Message const& query)
{
	socket_->write(reinterpret_cast<char const*>(query.data()), query.size());
}

void MinidspVolumePlugin::handleReply_()
{
	QByteArray reply = socket_->readAll();
	auto rep = minidsp::Reply{reinterpret_cast<unsigned char*>(reply.data()), static_cast<std::uint32_t>(reply.size())};
	if(rep.type() == minidsp::Reply::Type::VolumeReply)
	{
		currentVolumeStored_ = rep.volume();
		emit currentVolumeChanged(currentVolume_());
		emit valueValidChanged(true);
	}
	else if(rep.type() == minidsp::Reply::Type::DeviceInformationReply)
	{
		currentVolumeStored_ = rep.volume();
		mutedStored_ = rep.muted();
		emit currentVolumeChanged(currentVolume_());
		emit valueValidChanged(true);
		emit mutedChanged(mutedStored_);
	}
	else if(rep.type() == minidsp::Reply::Type::MuteReply)
	{
		mutedStored_ = !mutedStored_;
		emit mutedChanged(mutedStored_);
	}
	if(!pendingQueries_.empty())
		executeNextQuery_();
}

void MinidspVolumePlugin::handleConnected_()
{
	setConnected(true);
	refreshVolume_();
}

void MinidspVolumePlugin::handleDisconnected_()
{
	setConnected(false);
	pendingQueries_.clear();
}

}
}
}

