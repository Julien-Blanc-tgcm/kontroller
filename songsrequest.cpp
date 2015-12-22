#include "songsrequest.h"
#include "client.h"
#include "kodifile.h"

SongsRequest::SongsRequest(QObject *parent) : QObject(parent),
  success(false)
{

}

void SongsRequest::start(int albumid)
{
    QJsonObject filter;
    QJsonObject parameters;
    if(albumid != 0)
    {
        filter["albumid"] = albumid;
        parameters.insert("filter", filter);
    }
    auto message = QJsonRpcMessage::createRequest("AudioLibrary.GetSongs", parameters);
    QJsonRpcServiceReply* reply = Client::current().send(message);
    if(reply)
        connect(reply, &QJsonRpcServiceReply::finished, this, &SongsRequest::parseSongsResult);
    else
        emit finished();
}

void SongsRequest::parseSongsResult()
{
    QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        results.clear();
        QJsonRpcMessage response = reply->response();
        QJsonObject obj = response.toObject();
        if(obj.find("result") != obj.end())
        {
            QJsonValue result = obj.take("result");
            if(result.type() == QJsonValue::Object)
            {
                QJsonValue files;
                files = result.toObject().take("songs");
                if(files.type() == QJsonValue::Array)
                {
                    QJsonArray res = files.toArray();
                    for(QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
                    {
                        KodiFile* file = new KodiFile();
                        if((*it).type() == QJsonValue::Object)
                        {
                            QJsonObject obj = (*it).toObject();
                            QJsonValue val = obj.value("label");
                            if(val.type() == QJsonValue::String)
                                file->setLabel(val.toString());
                            val = obj.value("songid");
                            if(val.type() == QJsonValue::Double)
                                file->setFile(QString::number(val.toDouble()));
                            file->setFiletype("song");
                            file->setType("song");
                            results.push_back(file);
                        }
                    }
                }
            }
        }
        success = true;
    }
    emit finished();
}

SongsRequest::~SongsRequest()
{
    for(auto res : results)
        res->deleteLater();
    results.clear();
}
