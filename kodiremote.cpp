#include "client.h"
#include "kodiremote.h"

KodiRemote::KodiRemote(QObject *parent) :
    QObject(parent)
{
}

void KodiRemote::back()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Back");
    Client::current().send(message);
}

void KodiRemote::contextMenu()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ContextMenu");
    Client::current().send(message);
}

void KodiRemote::down()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Down");
    Client::current().send(message);
}

void KodiRemote::home()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Home");
    Client::current().send(message);
}

void KodiRemote::info()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Info");
    Client::current().send(message);
}

void KodiRemote::left()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Left");
    Client::current().send(message);
}

void KodiRemote::right()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Right");
    Client::current().send(message);
}

void KodiRemote::select()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Select");
    Client::current().send(message);
}

void KodiRemote::showCodec()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ShowCodec");
    Client::current().send(message);
}

void KodiRemote::showOSD()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ShowOSD");
    Client::current().send(message);
}

void KodiRemote::up()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Up");
    Client::current().send(message);
}
