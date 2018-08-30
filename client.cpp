#include "client.h"
#include <QSettings>
#include "settingsmanager.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

Client::Client(QObject *parent) :
    QObject(parent),
    serverPort_(9090),
    serverHttpPort_(8080),
    serverName_(),
    client_(nullptr),
    clientSocket_(nullptr),
    tcpClient_(nullptr),
    connectionStatus_(0)
{
}

Client::~Client()
{
    freeConnections();
}

void Client::freeConnections()
{
    if(client_)
        client_->deleteLater();
    client_ = nullptr;
    if(clientSocket_)
        clientSocket_->deleteLater();
    clientSocket_ = nullptr;
    if(tcpClient_)
        tcpClient_->deleteLater();
    tcpClient_ = nullptr;
}

Client& Client::current()
{
    static Client instance_;
    return instance_;
}

QString Client::serverAddress() const
{
    return serverAddress_;
}

int Client::serverPort() const
{
    return serverPort_;
}

int Client::serverHttpPort() const
{
    return serverHttpPort_;
}

void Client::refresh()
{
    freeConnections();
    Server* server = nullptr;
    if(serverName_.size() > 0)
        server = SettingsManager::instance().server(serverName_);
    else if(SettingsManager::instance().servers().size() >= 1)
    {
        server = SettingsManager::instance().servers().front().get();
        serverName_ = server->name();
    }
    if(server)
    {
        serverAddress_ = server->serverAddress();
        serverPort_ = server->serverPort();
        serverHttpPort_ = server->serverHttpPort();
        qDebug() << "Connection to " << serverAddress_ << serverPort_;
        if(serverAddress_.size() > 0 && serverPort_ > 0)
        {
            setConnectionStatus(1);
//            if(server->useHttpInterface())
//            {
//                client_ = new QJsonRpcHttpClient("http://" + serverAddress_ + ":" + QString::number(serverPort_) + "/jsonrpc");
//            }
//            else
            {
                clientSocket_ = new QTcpSocket();
                connect(clientSocket_, SIGNAL(connected()), this, SLOT(handleConnectionSuccess()));
                connect(clientSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleConnectionError(QAbstractSocket::SocketError)));
                clientSocket_->connectToHost(serverAddress_, serverPort_);
            }
        }
    }
    else
        setConnectionStatus(-1);
}

int Client::connectionStatus() const
{
    return connectionStatus_;
}

bool Client::useHttpInterface() const
{
    return false;
}

Server *Client::server()
{
    for(auto& server : SettingsManager::instance().servers())
    {
        if(server->name() == serverName_)
            return server.get();
    }
    return nullptr;
}

void Client::switchToServer(const QString &serverName)
{
    serverName_ = serverName;
    emit serverChanged();
    refresh();
}

void Client::handleError(QJsonRpcMessage error)
{
    if(error.errorCode() == QJsonRpc::ErrorCode::TimeoutError)
    {
        //setConnectionStatus(0);
    }
    else if(error.errorMessage().startsWith("error with http request"))
    {
        setConnectionStatus(0);
    }
    else
    {
        qDebug() << error;
        qDebug() << error.errorMessage();
    }
}

QJsonRpcServiceReply* Client::send(QJsonRpcMessage message)
{
    if(connectionStatus_ == 0)
    {
        setConnectionStatus(1);
        refresh();
    }
    if(tcpClient_)
    {
/*        auto reply = tcpClient_->sendMessage(message);
        connect(reply, SIGNAL(finished()), this, SLOT(handleReplyFinished()));
        return reply; */
        return httpSend(message);
    }
    else
        return nullptr;
}

QJsonRpcServiceReply *Client::httpSend(QJsonRpcMessage message)
{
    if(!client_)
    {
        client_ = new QJsonRpcHttpClient("http://" + serverAddress_ + ":" + QString::number(serverHttpPort_) + "/jsonrpc");
    }
    auto reply = client_->sendMessage(message);
    connect(reply, SIGNAL(finished()), this, SLOT(handleReplyFinished()));
    return reply;
}

void Client::handleReplyFinished()
{
    QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        auto response = reply->response();
        if(response.type() == QJsonRpcMessage::Error)
        {
            qDebug() << reply->response();
            handleError(reply->response());
        }
    }
    else
        setConnectionStatus(0);
    reply->deleteLater();
}

void Client::setConnectionStatus(int connectionStatus)
{
    connectionStatus_ = connectionStatus;
    emit connectionStatusChanged(connectionStatus_);
}

void Client::handleConnectionSuccess()
{
    tcpClient_ = new QJsonRpcSocket(clientSocket_);
    connect(tcpClient_, SIGNAL(messageReceived(QJsonRpcMessage)), this, SLOT(handleMessageReceived(QJsonRpcMessage)));
    setConnectionStatus(2);
    emit serverChanged();
}

void Client::handleConnectionError(QAbstractSocket::SocketError err)
{
    setConnectionStatus(0);
    qDebug() << err;
}

void Client::handleMessageReceived(QJsonRpcMessage message)
{
/*    QJsonValue result_ = message.result();
    if(!result_.isObject())
        return;
    QJsonObject result = result_.toObject(); */
    if(message.type() == QJsonRpcMessage::Notification)
    {
        QString method = message.method();
        if(method == "Player.OnPause" || method == "Player.OnPlay")
        {
            QJsonObject data = message.params().toObject().value("data").toObject();
            QJsonValue player = data.value("player");
            int playerId;
            int speed;
            int itemId = -1;
            if(!player.isObject())
                return;
            auto playerIdVal = player.toObject().value("playerid");
            if(!playerIdVal.isDouble())
                return;
            playerId = (int)playerIdVal.toDouble();
            auto speedVal = player.toObject().value("speed");
            if(!speedVal.isDouble())
                return;
            speed = (int)speedVal.toDouble();
            emit playerSpeedChanged(playerId, speed);
            QJsonObject item = data.value("item").toObject();
            QJsonValue id = item.value("id");
            QString type = item.value("type").toString();
            if(id.isDouble())
                itemId = (int)id.toDouble();
            emit playlistCurrentItemChanged(playerId, type, itemId);
            qDebug() << message;
        }
        else if(method == "Player.OnStop")
        {
            emit playerStopped();
        }
        else if(method == "Playlist.OnClear")
        {
            QJsonValue val = message.params().toObject().value("data").toObject().value("playlistid");
            if(val.isDouble())
            {
                emit playlistCleared((int)val.toDouble());
            }
        }
        else if(method == "Playlist.OnRemove")
        {
            QJsonObject data = message.params().toObject().value("data").toObject();
            if(!data.isEmpty())
            {
                QJsonValue playlistId = data.value("playlistid");
                QJsonValue position = data.value("position");
                if(playlistId.isDouble() && position.isDouble())
                    emit playlistElementRemoved((int)playlistId.toDouble(), (int)position.toDouble());
            }
        }
        else if(method == "Playlist.OnAdd")
        {
            QJsonObject data = message.params().toObject().value("data").toObject();
            if(!data.isEmpty())
            {
                QJsonValue playlistId = data.value("playlistid");
                if(playlistId.isDouble())
                    emit playlistElementAdded((int)playlistId.toDouble());
            }
        }
        else if(method == "Player.OnSeek")
        {
            QJsonObject data = message.params().toObject().value("data").toObject();
            if(!data.isEmpty())
            {
                QJsonObject player = data.value("player").toObject();
                if(!player.isEmpty())
                {
                    int playerId = player.value("playerId").toDouble();
                    QJsonObject offset = player.value("seekoffset").toObject();
                    if(!offset.isEmpty())
                    {
                        int hours = offset.value("hours").toDouble();
                        int minutes = offset.value("minutes").toDouble();
                        int seconds = offset.value("seconds").toDouble();
                        int milliseconds = offset.value("milliseconds").toDouble();
                        emit playerSeekChanged(playerId, hours, minutes, seconds, milliseconds);
                    }
                }
            }
        }
        else if(method == "Input.OnInputRequested")
        {
            auto data = message.params().toObject().take("data").toObject();
            emit inputRequested(data["title"].toString(),
                    data["type"].toString(),
                    data["value"].toString());
        }
        else if(method == "Input.OnInputFinished")
        {
            emit inputFinished();
        }
        else
            qDebug() << message;
    }
}

}
}
}
