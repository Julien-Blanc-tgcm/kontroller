#include "kodiclient.h"
#include <QSettings>
#include "kodisettingsmanager.h"

KodiClient::KodiClient(QObject *parent) :
    QObject(parent),
    serverPort_(8080),
    client_(nullptr),
    clientSocket_(nullptr),
    tcpClient_(nullptr),
    connectionStatus_(0)
{
}

KodiClient::~KodiClient()
{
    freeConnections();
}

void KodiClient::freeConnections()
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

KodiClient& KodiClient::current()
{
    static KodiClient instance_;
    return instance_;
}

void KodiClient::setServerAddress(QString address)
{
    if(serverAddress_ != address)
    {
        serverAddress_ = address;
        emit serverAddressChanged(serverAddress_);
    }
}

QString KodiClient::serverAddress() const
{
    return serverAddress_;
}

void KodiClient::setServerPort(int port)
{
    if(serverPort_ != port)
    {
        serverPort_ = port;
        emit serverPortChanged(serverPort_);
    }
}

int KodiClient::serverPort() const
{
    return serverPort_;
}

void KodiClient::refresh()
{
    freeConnections();
    if(serverAddress_.size() > 0 && serverPort_ > 0)
    {
        setConnectionStatus(1);
        if(KodiSettingsManager::instance().useHttpInterface())
        {
            client_ = new QJsonRpcHttpClient("http://" + serverAddress_ + ":" + QString::number(serverPort_) + "/jsonrpc");
        }
        else
        {
            clientSocket_ = new QTcpSocket();
            connect(clientSocket_, SIGNAL(connected()), this, SLOT(handleConnectionSuccess()));
            connect(clientSocket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleConnectionError(QAbstractSocket::SocketError)));
            clientSocket_->connectToHost(serverAddress_, serverPort_);
        }
    }
}

int KodiClient::connectionStatus() const
{
    return connectionStatus_;
}

void KodiClient::handleError(QJsonRpcMessage error)
{
    if(error.errorCode() == QJsonRpc::ErrorCode::TimeoutError)
    {
        setConnectionStatus(0);
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

QJsonRpcServiceReply* KodiClient::send(QJsonRpcMessage message)
{
    if(connectionStatus_ == 0)
        setConnectionStatus(1);
    if(client_)
    {
        auto reply = client_->sendMessage(message);
        connect(reply, SIGNAL(finished()), this, SLOT(handleReplyFinished()));
        return reply;
    }
    else if(tcpClient_)
    {
        auto reply = tcpClient_->sendMessage(message);
        connect(reply, SIGNAL(finished()), this, SLOT(handleReplyFinished()));
        return reply;
    }
    else
        return nullptr;
}

void KodiClient::handleReplyFinished()
{
    QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        if(reply->response().errorCode() != 0)
            handleError(reply->response());
        else
        {
            if(connectionStatus_ != 2)
                setConnectionStatus(2);
        }
    }
    else
        setConnectionStatus(0);
    reply->deleteLater();
}

void KodiClient::setConnectionStatus(int connectionStatus)
{
    connectionStatus_ = connectionStatus;
    emit connectionStatusChanged(connectionStatus_);
}

void KodiClient::handleConnectionSuccess()
{
    tcpClient_ = new QJsonRpcSocket(clientSocket_);
    connect(tcpClient_, SIGNAL(messageReceived(QJsonRpcMessage)), this, SLOT(handleMessageReceived(QJsonRpcMessage)));
    setConnectionStatus(2);
}

void KodiClient::handleConnectionError(QAbstractSocket::SocketError err)
{
    setConnectionStatus(0);
    qDebug() << err;
}

void KodiClient::handleMessageReceived(QJsonRpcMessage message)
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
            QJsonValue player = message.params().toObject().value("data").toObject().value("player");
            if(player.isObject())
            {
                auto playerIdVal = player.toObject().value("playerid");
                if(!playerIdVal.isDouble())
                    return;
                int playerid = (int)playerIdVal.toDouble();
                auto speedVal = player.toObject().value("speed");
                if(!speedVal.isDouble())
                    return;
                int speed = (int)speedVal.toDouble();
                emit playerSpeedChanged(playerid, speed);
            }
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
        else
            qDebug() << message;
    }
}
