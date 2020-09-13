#include "systemservice.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

SystemService::SystemService(QObject *parent) : QObject(parent)
{

}

Client* SystemService::client() const
{
	return client_;
}

void SystemService::shutdownServer()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("System.Shutdown");
	client_->send(message);
}

void SystemService::rebootServer()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("System.Reboot");
	client_->send(message);
}

void SystemService::suspendServer()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("System.Suspend");
	client_->send(message);
}

void SystemService::hibernateServer()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("System.Hibernate");
	client_->send(message);
}

void SystemService::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	emit clientChanged(client_);
}

}
}
}
