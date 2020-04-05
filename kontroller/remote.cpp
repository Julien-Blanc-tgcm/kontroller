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

void Remote::back()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Back");
	Client::current().send(message);
}

void Remote::contextMenu()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ContextMenu");
	Client::current().send(message);
}

void Remote::down()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Down");
	Client::current().send(message);
}

void Remote::home()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Home");
	Client::current().send(message);
}

void Remote::info()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Info");
	Client::current().send(message);
}

void Remote::left()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Left");
	Client::current().send(message);
}

void Remote::right()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Right");
	Client::current().send(message);
}

void Remote::select()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Select");
	Client::current().send(message);
}

void Remote::showCodec()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ShowCodec");
	Client::current().send(message);
}

void Remote::showOSD()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ShowOSD");
	Client::current().send(message);
}

void Remote::up()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Up");
	Client::current().send(message);
}

void Remote::volumeUp()
{
	Client::current().server()->volumePlugin()->increaseVolume();
}

void Remote::volumeDown()
{
	Client::current().server()->volumePlugin()->decreaseVolume();
}

}
}
}
