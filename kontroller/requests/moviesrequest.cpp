#include "moviesrequest.hpp"

#include "../file.h"
#include "../utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

MoviesRequest::MoviesRequest(Client* client, QObject* parent) : QObject(parent), client_{client}
{
}

void MoviesRequest::start()
{
	QJsonObject obj;
	obj["order"] = QLatin1String("ascending");
	obj["method"] = QLatin1String("label");
	obj["ignorearticle"] = client_->sortIgnoreArticle();
	QJsonObject parameters;
	parameters.insert("sort", obj);
	QJsonArray properties;
	properties.push_back(QString::fromUtf8("thumbnail"));
	properties.push_back(QString::fromUtf8("lastplayed"));
	properties.push_back(QString::fromUtf8("resume"));
	parameters.insert("properties", properties);
	auto message = QJsonRpcMessage::createRequest("VideoLibrary.GetMovies", parameters);
	QJsonRpcServiceReply* reply = client_->send(message);
	if (reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &MoviesRequest::parseResult_);
}

void MoviesRequest::parseResult_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		QJsonRpcMessage response = reply->response();
		QJsonObject obj = response.toObject();
		if (obj.find("result") != obj.end())
		{
			QJsonValue result = obj.take("result");
			if (result.type() == QJsonValue::Object)
			{
				QJsonValue files;
				files = result.toObject().take("movies");
				if (files.type() == QJsonValue::Array)
				{
					QJsonArray const res = files.toArray();
					for (auto const& f : res)
					{
						File file;
						if (f.type() == QJsonValue::Object)
						{
							QJsonObject obj = f.toObject();
							QJsonValue val = obj.value("label");
							if (val.type() == QJsonValue::String)
								file.setLabel(val.toString());
							val = obj.value("movieid");
							if (val.type() == QJsonValue::Double)
								file.setId(static_cast<int>(val.toDouble()));
							file.setFile(obj.value("file").toString());
							file.setFiletype("movie");
							file.setType("movie");
							file.setIcon("movie");
							file.setThumbnail(getImageUrl(client_, obj.value("thumbnail").toString()).toString());
							val = obj.value("lastplayed");
							file.setPlayed(val.isString() && val.toString().size() > 0);
							val = obj.value("resume");
							if (val.isObject())
							{
								auto resumeObj = val.toObject();
								auto position = resumeObj.value("position");
								if (position.isDouble() && position.toDouble() != 0)
								{
									file.setPlayed(false); // currently playing, so mark as not played
								}
							}
							results.push_back(file);
						}
					}
				}
			}
		}
	}
	success = true;
	emit finished();
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
