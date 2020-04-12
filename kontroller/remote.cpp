#include "client.h"
#include "remote.h"
#include "server.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

Remote::Remote(QObject *parent) :
    QObject(parent)
{
}

Client* Remote::client() const
{
	return client_;
}

void Remote::back()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Back");
	client_->send(message);
}

void Remote::contextMenu()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ContextMenu");
	client_->send(message);
}

void Remote::down()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Down");
	client_->send(message);
}

void Remote::home()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Home");
	client_->send(message);
}

void Remote::info()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Info");
	client_->send(message);
}

void Remote::left()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Left");
	client_->send(message);
}

void Remote::right()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Right");
	client_->send(message);
}

void Remote::select()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Select");
	client_->send(message);
}

void Remote::showCodec()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ShowCodec");
	client_->send(message);
}

void Remote::showOSD()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ShowOSD");
	client_->send(message);
}

void Remote::up()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Up");
	client_->send(message);
}

void Remote::volumeUp()
{
	client_->server()->volumePlugin()->increaseVolume();
}

void Remote::volumeDown()
{
	client_->server()->volumePlugin()->decreaseVolume();
}

void Remote::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	emit clientChanged(client_);
}

}
}
}
