#include "sbzeroconfservicediscovery.h"

//#include "mdns.h"

#include <QTimer>
#include <QUdpSocket>
#include <arpa/inet.h>
#include <memory>

#include <QDnsLookup>
#include "dnspacket.h"

namespace
{
QHostAddress mdnsAddressIpv4 = QHostAddress{"224.0.0.251"};
QHostAddress mdnsAddressIpv6 = QHostAddress{"ff02::fb"};
quint16 mdnsPort = 5353;
} // namespace

class SBZeroConfServiceDiscoveryPrivate
{
	Q_DISABLE_COPY(SBZeroConfServiceDiscoveryPrivate)
	Q_DECLARE_PUBLIC(SBZeroConfServiceDiscovery)
	SBZeroConfServiceDiscovery* q_ptr;
	QVector<QUdpSocket*> sockets;
	QNetworkInterface interface;
	QTimer timer;
	quint16 transactionId = 0;
	SBZeroConfServiceDiscoveryPrivate(SBZeroConfServiceDiscovery* owner);
	QByteArray getMdnsQuery(QString serviceName);
};

SBZeroConfServiceDiscoveryPrivate::SBZeroConfServiceDiscoveryPrivate(SBZeroConfServiceDiscovery* owner) : q_ptr{owner}
{
}

QByteArray SBZeroConfServiceDiscoveryPrivate::getMdnsQuery(QString serviceName)
{
	QByteArray array;
	array.reserve(2048);
	if (transactionId == std::numeric_limits<quint16>::max())
		transactionId = 1;
	else
		transactionId += 1;
	array.push_back(htons(transactionId) & 0xffu); // transaction id
	array.push_back(htons(transactionId) >> 8u);
	array.push_back('\0'); // checkdisable
	array.push_back('\0');
	array.push_back('\0'); // 1 query
	array.push_back(static_cast<qint8>(1));
	array.push_back('\0'); // 0 answer
	array.push_back('\0');
	array.push_back('\0'); // 0 auth rr
	array.push_back('\0');
	array.push_back('\0'); // 0 add rr
	array.push_back('\0');
	auto items = serviceName.split('.', QString::SkipEmptyParts);
	for (auto& item : items) // data string part
	{
		array.push_back(static_cast<quint8>(item.size()));
		array.push_back(item.toUtf8());
	}
	array.push_back('\0'); // null terminator of string part
	array.push_back('\0'); // query type
	array.push_back(QDnsLookup::PTR);
	array.push_back('\0'); // QM type - 0x80); // QU type
	array.push_back(1);    //
	return array;
}

SBZeroConfServiceDiscovery::SBZeroConfServiceDiscovery(QObject* parent) :
    QObject{parent}, d_ptr{new SBZeroConfServiceDiscoveryPrivate{this}}
{
	Q_D(SBZeroConfServiceDiscovery);
	d->timer.setSingleShot(true);
}

void SBZeroConfServiceDiscovery::setInterface(QNetworkInterface interface)
{
	Q_D(SBZeroConfServiceDiscovery);
	d->interface = interface;
	connect(&d->timer, &QTimer::timeout, this, &SBZeroConfServiceDiscovery::finishDiscovery_);
}

SBZeroConfServiceDiscovery::~SBZeroConfServiceDiscovery() noexcept = default;

void SBZeroConfServiceDiscovery::query(QString serviceName, std::chrono::milliseconds timeout)
{
	Q_D(SBZeroConfServiceDiscovery);
	if (!d->interface.isValid())
	{
		emit discoveryError("Invalid interface");
		return;
	}
	if (d->timer.isActive())
	{
		emit discoveryError("Already in proress");
		return;
	}
	for (auto& address : d->interface.addressEntries())
	{
		auto s = new QUdpSocket(this);
		d->sockets.push_back(s);
		QHostAddress mdnsAddress;
		QHostAddress bindAddress;
		if (address.ip().protocol() == QAbstractSocket::IPv4Protocol)
		{
			mdnsAddress = mdnsAddressIpv4;
			bindAddress = address.ip();
		}
		else if (address.ip().protocol() == QAbstractSocket::IPv6Protocol)
		{
			mdnsAddress = mdnsAddressIpv6;
			bindAddress = address.ip();
		}
		else
			continue;
		auto ret = s->bind(bindAddress, 0, QUdpSocket::ShareAddress);
		if (!ret)
		{
			qDebug() << "Could not bind to port " << mdnsPort << ", error is " << s->errorString();
			ret = s->bind(0); // mdns port not available, try to bind to another port
		}
		qDebug() << "Socket bind returned " << ret << ", bound to port " << s->localPort() << " and address"
		         << address.ip() << " and interface " << d->interface;
		//		s->setMulticastInterface(d->interface);
		//		s->joinMulticastGroup(mdnsAddress, d->interface);
		s->joinMulticastGroup(mdnsAddress);
		connect(s, &QUdpSocket::readyRead, this, &SBZeroConfServiceDiscovery::parseData_);
		s->writeDatagram(d->getMdnsQuery(serviceName), mdnsAddress, mdnsPort);
		//	}
		d->timer.start(timeout.count());
	}
}

void SBZeroConfServiceDiscovery::interruptCurrentOperation()
{
	Q_D(SBZeroConfServiceDiscovery);
	if (d->timer.isActive())
		d->timer.start(0); // make the current operation finish immediately
}

void SBZeroConfServiceDiscovery::parseData_()
{
	if (auto sender = dynamic_cast<QUdpSocket*>(QObject::sender()))
	{
		while (sender->hasPendingDatagrams())
		{
			// QByteArray data = sender->readAll();
			// only available with Qt >= 5.8
			//			auto datagram = sender->receiveDatagram();

			// alternative code for Qt 5.6
			QHostAddress hostAddress;
			quint16 hostPort = 0;
			QByteArray data{static_cast<int>(sender->pendingDatagramSize()), static_cast<char>(0)};
			auto size = sender->readDatagram(data.data(), data.size(), &hostAddress, &hostPort);
			SBZeroConfServiceRecord record;
			record.setAddress(hostAddress.toString()); //datagram.senderAddress().toString());
			if (size < 14)
			{
				qDebug() << "Invalid data received";
				continue;
			}
			qDebug() << data;
			auto packet_opt = DNSPacket::parse_from(data.data(), data.size());
			if (!static_cast<bool>(packet_opt) || packet_opt->first.query_count() != 1 ||
			    packet_opt->first.answers_count() == 0)
			{
				qDebug() << "Invalid packet received";
				if (static_cast<bool>(packet_opt))
				{
					DNSPacket& pack = packet_opt->first;
					qDebug() << pack.id() << pack.flags() << pack.query_count() << pack.answers_count()
					         << pack.authority_count() << pack.additional_count();
				}
				continue;
			}
			DNSPacket& packet = packet_opt->first;
			char* remaining = data.data() + packet_opt->second;
			int remaining_size = data.size() - packet_opt->second;
			// now, parse the query. There is only one (checked before)
			auto query = packet.parse_next_query(remaining, remaining_size);
			if (!static_cast<bool>(query))
			{
				qDebug() << "Invalid1";
				continue;
			}
			qDebug() << packet.id() << packet.flags() << packet.query_count() << packet.answers_count()
			         << packet.authority_count() << packet.additional_count() << query->first.name.c_str();
			remaining = remaining + query->second;
			remaining_size -= query->second;
			bool error = false;
			bool portOk = false;
			bool nameOk = false;
			bool addressOk = false;
			for (int i = 0; i < packet.answers_count(); ++i)
			{
				auto answer = packet.parse_next_answer(remaining, remaining_size);
				if (std::holds_alternative<std::monostate>(answer.first))
				{ // unhandled dns record, just ignore it
					qDebug() << "Ignoring dns answer" << i;
				}
				else if (std::holds_alternative<DNSAnswer_TypePtr>(answer.first))
				{
					auto& a = std::get<DNSAnswer_TypePtr>(answer.first);
					qDebug() << a.domain_name.c_str();
					// do nothing from this one
				}
				else if (std::holds_alternative<DNSAnswer_TypeTxt>(answer.first))
				{
					// do nothing from this one
					auto& a = std::get<DNSAnswer_TypeTxt>(answer.first);
					for (auto& txt : a.txts)
						qDebug() << txt.c_str();
				}
				else if (std::holds_alternative<DNSAnswer_TypeSrv>(answer.first))
				{
					// we take the port and the address from this one
					auto& a = std::get<DNSAnswer_TypeSrv>(answer.first);
					qDebug() << a.target.c_str() << a.port;
					record.setPort(a.port);
					portOk = true;
					record.setHostname(QString::fromStdString(a.target));
					nameOk = true;
				}
				else if (std::holds_alternative<DNSAnswer_TypeA>(answer.first))
				{
					// we take the address from this one
					auto& a = std::get<DNSAnswer_TypeA>(answer.first);
					qDebug() << QHostAddress(a.ipAddress);
					record.setAddress(QHostAddress(a.ipAddress).toString());
					addressOk = true;
				}
				remaining = remaining + answer.second;
				remaining_size -= answer.second;
				// TODO : parse answers
				//				auto type = static_cast<quint16>((data[i] << 8) + data[i + 1]);
				//				auto query_qu = static_cast<quint16>((data[i + 2] << 8) + data[i + 3]);
				//				auto resp_name = static_cast<quint16>((data[i + 4] << 8) + data[i + 5]);
				//				auto resp_type = static_cast<quint16>((data[i + 6] << 8) + data[i + 7]);
				//				auto cache_flush = static_cast<quint16>((data[i + 8] << 8) + data[i + 9]);
				//				auto ttl =
				//				    static_cast<quint16>((data[i + 10] << 24) + (data[i + 11] << 16) + (data[i + 12] <<
				// 8)
				//+ data[i + 13]); 				auto resp_len = static_cast<quint16>((data[i + 14] << 8) + data[i +
				// 15]); 				Q_UNUSED(type); 				Q_UNUSED(query_qu);
				// Q_UNUSED(resp_name); Q_UNUSED(resp_type);
				//				Q_UNUSED(cache_flush);
				//				Q_UNUSED(ttl);
				//				start = i + 16;
				//				QString hostName = parseDnsPtrDomainName_(data, resp_len, start, i);
				//				record.setHostname(hostName);
				//				qDebug() << hostName;
			}
			if (error)
				continue;
			if (addressOk && portOk && nameOk)
			{
				emit serviceDiscovered(record);
			}
		}
	}
}

void SBZeroConfServiceDiscovery::finishDiscovery_()
{
	Q_D(SBZeroConfServiceDiscovery);
	for (auto s : d->sockets)
	{
		bool v = s->hasPendingDatagrams();
		if (v)
			qDebug() << "WTF !!!";
		s->deleteLater();
	}
	d->sockets.clear();
	emit discoveryFinished();
}
