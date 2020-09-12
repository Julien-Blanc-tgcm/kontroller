#include "sbzeroconfservicerecord.h"

SBZeroConfServiceRecord::SBZeroConfServiceRecord()
{

}

QString SBZeroConfServiceRecord::service() const
{
	return service_;
}

QString SBZeroConfServiceRecord::hostname() const
{
	return hostname_;
}

QString SBZeroConfServiceRecord::address() const
{
	return hostAddress_.toString();
}

int SBZeroConfServiceRecord::port() const
{
	return port_;
}

void SBZeroConfServiceRecord::setService(QString service)
{
	service_ = service;
}

void SBZeroConfServiceRecord::setHostname(QString hostname)
{
	hostname_ = hostname;
}

void SBZeroConfServiceRecord::setAddress(QString address)
{
	hostAddress_ = QHostAddress{address};
}

void SBZeroConfServiceRecord::setPort(int port)
{
	port_ = port;
}
