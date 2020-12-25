#include "tvshowsrequest.hpp"

#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

TvShowsRequest::TvShowsRequest(Client* client, QObject *parent) : QObject(parent), client_{client}
{
}

void TvShowsRequest::start()
{
	QJsonObject obj;
	obj["order"] = QLatin1String("ascending");
	obj["method"] = QLatin1String("label");
	obj["ignorearticle"] = client_->sortIgnoreArticle();
	QJsonObject parameters;
	parameters.insert("sort", obj);
	QJsonArray properties;
	properties.push_back(QString::fromUtf8("thumbnail"));
	parameters.insert("properties", properties);
	auto message = QJsonRpcMessage::createRequest("VideoLibrary.GetTVShows", parameters);
	auto reply = client_->send(message);
	if (reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &TvShowsRequest::parseResult_);
}

void TvShowsRequest::parseResult_()
{
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		results.clear();
		QJsonRpcMessage response = reply->response();
		QJsonObject obj = response.toObject();
		if (obj.find("result") != obj.end())
		{
			QJsonValue result = obj.take("result");
			if (result.type() == QJsonValue::Object)
			{
				QJsonValue files;
				files = result.toObject().take("tvshows");
				if (files.type() == QJsonValue::Array)
				{
					QJsonArray res = files.toArray();
					for (QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
					{
						File file;
						if ((*it).type() == QJsonValue::Object)
						{
							QJsonObject obj = (*it).toObject();
							QJsonValue val = obj.value("label");
							if (val.type() == QJsonValue::String)
								file.setLabel(val.toString());
							val = obj.value("tvshowid");
							if (val.type() == QJsonValue::Double)
								file.setId(static_cast<int>(val.toDouble()));
							file.setFile(obj.value("file").toString());
							file.setFiletype("tvshow");
							file.setType("tvshow");
							file.setIcon("tvshow");
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

} // namespace kontroller
} // namespace tgcm
} // namespace eu
