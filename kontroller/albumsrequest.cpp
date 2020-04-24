#include "albumsrequest.h"
#include "client.h"
#include "file.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{
AlbumsRequest::AlbumsRequest(Client* client, QObject *parent) : QObject(parent),
    client_{client},
    success(false)
{

}

void AlbumsRequest::start(int artistid)
{
	QJsonObject filter;
	QJsonObject parameters;
	if(artistid != 0)
	{
		filter["artistid"] = artistid;
		parameters.insert("filter", filter);
	}
	QJsonArray properties;
	properties.append(QLatin1String("thumbnail"));
	parameters.insert("properties", properties);
	QJsonObject sort;
	sort["order"] = QLatin1String("ascending");
	sort["method"] = QLatin1String("label");
	sort["ignorearticle"] = true;
	parameters.insert("sort", sort);
	auto message = QJsonRpcMessage::createRequest("AudioLibrary.GetAlbums", parameters);
	QJsonRpcServiceReply* reply = client_->send(message);
	if(reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &AlbumsRequest::parseAlbumsResult);
	else
		emit finished();
}

void AlbumsRequest::startWithGenre(int genreid)
{
	QJsonObject filter;
	QJsonObject parameters;
	filter["genreid"] = genreid;
	parameters.insert("filter", filter);
	QJsonArray properties;
	properties.append(QLatin1String("thumbnail"));
	parameters.insert("properties", properties);
	auto message = QJsonRpcMessage::createRequest("AudioLibrary.GetAlbums", parameters);
	QJsonRpcServiceReply* reply = client_->send(message);
	if(reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &AlbumsRequest::parseAlbumsResult);
	else
		emit finished();
}

void AlbumsRequest::parseAlbumsResult()
{
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply)
	{
		QJsonRpcMessage response = reply->response();
		QJsonObject obj = response.toObject();
		if(obj.find("result") != obj.end())
		{
			QJsonValue result = obj.take("result");
			if(result.type() == QJsonValue::Object)
			{
				QJsonValue files;
				files = result.toObject().take("albums");
				if(files.type() == QJsonValue::Array)
				{
					QJsonArray res = files.toArray();
					for(QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
					{
						File file;
						if((*it).type() == QJsonValue::Object)
						{
							QJsonObject obj = (*it).toObject();
							QJsonValue val = obj.value("label");
							if(val.type() == QJsonValue::String)
								file.setLabel(val.toString());
							val = obj.value("albumid");
							if(val.type() == QJsonValue::Double)
								file.setFile(QString::number(val.toDouble()));
							file.setFiletype("album");
							file.setType("album");
							file.setThumbnail(getImageUrl(client_, obj.value("thumbnail").toString()).toString());
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

AlbumsRequest::~AlbumsRequest()
{
	results.clear();
}

}
}
}
