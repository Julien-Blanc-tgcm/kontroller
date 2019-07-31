#include "downloadservice.h"

#include "client.h"

#include <qjsonrpcservice.h>

namespace eu
{
namespace  tgcm
{
namespace kontroller
{

DownloadService::DownloadService(QObject *parent) : QObject(parent)
{

}

void DownloadService::addFile(QString filePath)
{
    QJsonObject parameters;
    parameters.insert("path", filePath);
    auto message = QJsonRpcMessage::createRequest("Files.PrepareDownload", parameters);
    auto reply = Client::current().send(message);
    if(reply)
        connect(reply, &QJsonRpcServiceReply::finished, this, &DownloadService::filePathRequestComplete_);
    else
    {
        // TODO : handle error
    }
}

void DownloadService::filePathRequestComplete_()
{
    QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        reply->deleteLater();
        auto response = reply->response();
        auto obj = response.toObject();
        if(obj.find("result") != obj.end())
        {
            auto result = obj.take("result");
            if(result.type() == QJsonValue::Object)
            {
                QString finalPath;
                auto details = result.toObject().take("details");
                if(details.type() == QJsonValue::Object)
                {
                    auto path = details.toObject().take("path");
                    if(path.type() != QJsonValue::String)
                    {
                        emit downloadError("", "Invalid path value");
                        return;
                    }
                    finalPath = path.toString();
                }
                auto protocol = result.toObject().take("protocol");
                if(protocol.type() != QJsonValue::String ||
                        protocol.toString() != "http")
                {
                    emit downloadError("", "Protocol not supported or invalid");
                    return;
                }
                auto mode = result.toObject().take("mode");
                if(mode.type() != QJsonValue::String ||
                        mode.toString() != "redirect")
                {
                    emit downloadError("", "Mode not supported or invalid");
                    return;
                }
                startDownloadHttp_(finalPath);
            }
            else
                emit downloadError("", "Invalid reply received");
        }
    }
    else
        emit downloadError("", "Invalid reply received");
}

void DownloadService::downloadCompleted_()
{
    auto reply = dynamic_cast<QNetworkReply*>(sender());
    if(reply)
    {
        reply->deleteLater();
        emit downloadCompleted("");
    }
}

void DownloadService::startDownloadHttp_(QString httppath)
{
    auto reply = Client::current().downloadFile(httppath);
    connect(reply, &QNetworkReply::finished, this, &DownloadService::downloadCompleted_);
}



}
}
}
