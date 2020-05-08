#include "songsrequest.h"
#include "client.h"
#include "file.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

SongsRequest::SongsRequest(Client* client, QObject *parent) : QObject(parent),
    success{false},
    client_{client}
{

}

void SongsRequest::start(int albumid)
{
	QJsonObject filter;
	QJsonObject parameters;
	QJsonArray properties;
	properties.push_back("title");
	properties.push_back("file");
	parameters["properties"] = properties;
	if(albumid != 0)
	{
		filter["albumid"] = albumid;
		parameters.insert("filter", filter);
		QJsonObject obj;
		obj.insert("order",  QString::fromUtf8("ascending"));
		obj.insert("method", QString::fromUtf8("track"));
		obj.insert("ignorearticle", client_->sortIgnoreArticle());
		parameters.insert("sort", obj);
	}
	else
	{
		QJsonObject obj;
		obj.insert("order",  QString::fromUtf8("ascending"));
		obj.insert("method", QString::fromUtf8("title"));
		obj.insert("ignorearticle", client_->sortIgnoreArticle());
		parameters.insert("sort", obj);
	}
	auto message = QJsonRpcMessage::createRequest("AudioLibrary.GetSongs", parameters);
	QJsonRpcServiceReply* reply = client_->send(message);
	if(reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &SongsRequest::parseSongsResult);
	else
		emit finished();
}

void SongsRequest::parseSongsResult()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
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
					QJsonArray const res = files.toArray();
					for(auto const & f : res)
					{
						File file;
						if(f.type() == QJsonValue::Object)
						{
							QJsonObject obj = f.toObject();
							QJsonValue val = obj.value("label");
							if(val.type() == QJsonValue::String)
								file.setLabel(val.toString());
							val = obj.value("songid");
							if(val.type() == QJsonValue::Double)
								file.setFile(QString::number(val.toDouble()));
							file.setFiletype("song");
							file.setType("song");
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
	results.clear();
}

}
}
}
