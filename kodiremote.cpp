#include "kodiclient.h"
#include "kodiremote.h"

KodiRemote::KodiRemote(QObject *parent) :
    QObject(parent)
{
}

void KodiRemote::back()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Back");
    KodiClient::current().send(message);
}

void KodiRemote::contextMenu()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ContextMenu");
    KodiClient::current().send(message);
}

void KodiRemote::down()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Down");
    KodiClient::current().send(message);
}

void KodiRemote::home()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Home");
    KodiClient::current().send(message);
}

void KodiRemote::info()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Info");
    KodiClient::current().send(message);
}

void KodiRemote::left()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Left");
    KodiClient::current().send(message);
}

void KodiRemote::right()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Right");
    KodiClient::current().send(message);
}

void KodiRemote::select()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Select");
    KodiClient::current().send(message);
}

void KodiRemote::showCodec()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ShowCodec");
    KodiClient::current().send(message);
}

void KodiRemote::showOSD()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.ShowOSD");
    KodiClient::current().send(message);
}

void KodiRemote::up()
{
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("Input.Up");
    KodiClient::current().send(message);
}
