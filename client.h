#ifndef EU_TGCM_KONTROLLER_CLIENT_H
#define EU_TGCM_KONTROLLER_CLIENT_H

#include <QObject>
#include <qjsonrpchttpclient.h>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Client : public QObject
{
    Q_OBJECT


    Q_PROPERTY(QString serverAddress READ serverAddress WRITE setServerAddress NOTIFY serverAddressChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged)

    QString serverAddress_;
    int serverPort_;
    int serverHttpPort_;
    QJsonRpcHttpClient* client_;
    QTcpSocket* clientSocket_;
    QJsonRpcSocket* tcpClient_;
    /**
     * @see connectionStatus()
     */
    int connectionStatus_;

    void freeConnections();
public:
    explicit Client(QObject *parent = 0);
    ~Client();

    static Client& current();

    void setServerAddress(QString address);

    QString serverAddress() const;

    void setServerPort(int port);

    int serverPort() const;

    int serverHttpPort() const;
    /**
     * @brief connectionStatus tells whether the client is connected
     * 0 means unconnected
     * 1 means connecting
     * 2 means connected
     * @return
     */
    int connectionStatus() const;
signals:
    void serverAddressChanged(QString address);
    void serverPortChanged(int port);
    void connectionStatusChanged(int connected);

public slots:
    void refresh();
    void handleError(QJsonRpcMessage error);
    /**
     * @brief send returns an object that can be used to wait for the query result. The kodi client will take care of releasing it
     * @param message the json rpc query.
     * @return
     */
    QJsonRpcServiceReply* send(QJsonRpcMessage message);

    /**
     * @brief httpSend does the same as send, but uses http transport. This is needed for ressources like images or files
     * @param message
     * @return
     */
    QJsonRpcServiceReply* httpSend(QJsonRpcMessage message);

private slots:
    void handleReplyFinished();
    void setConnectionStatus(int connectionStatus);

    void handleConnectionSuccess();
    void handleConnectionError(QAbstractSocket::SocketError);
    void handleMessageReceived(QJsonRpcMessage message);

signals:
    // these ones are the notifications the kodi api can send
    // note that if using HTTP transport, no notifications will be available
    void playerSpeedChanged(int playerid, int speed);
    void playlistCurrentItemChanged(int playerid, QString type, int id);
    // a player stopped (any player, information not in notification)
    void playerStopped();

    void playlistCleared(int playlistId);
    void playlistElementRemoved(int playlistId, int position);
    void playlistElementAdded(int playlistId);
    void playerSeekChanged(int playerId, int hours, int minutes, int seconds, int milliseconds);
};

}
}
}


#endif // EU_TGCM_KONTROLLER_CLIENT_H
