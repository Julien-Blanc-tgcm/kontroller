#include "tvshowepisodesrequest.h"

#include "../client.h"
#include "../file.h"
#include "../utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

TvShowEpisodesRequest::TvShowEpisodesRequest(Client* client, QObject* parent) : QObject(parent), client_{client}
{
}

TvShowEpisodesRequest::~TvShowEpisodesRequest() noexcept
{
	episodes.clear();
}

void TvShowEpisodesRequest::start(int tvshowId, int season)
{
	QJsonObject parameters;
	parameters.insert("season", season);
	parameters.insert("tvshowid", tvshowId);
	QJsonObject sort;
	sort.insert("order", QLatin1String("ascending"));
	sort.insert("method", QLatin1String("episode"));
	parameters.insert("sort", sort);
	QJsonArray properties;
	properties.push_back(QString::fromUtf8("thumbnail"));
	properties.push_back(QString::fromUtf8("lastplayed"));
	properties.push_back(QString::fromUtf8("resume"));
	parameters.insert("properties", properties);
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("VideoLibrary.GetEpisodes", parameters);
	QJsonRpcServiceReply* reply = client_->send(message);
	if (reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &TvShowEpisodesRequest::parseEpisodesResult_);
}

void TvShowEpisodesRequest::parseEpisodesResult_()
{
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
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
				files = result.toObject().take("episodes");
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
							val = obj.value("episodeid");
							if (val.type() == QJsonValue::Double)
								file.setId(static_cast<int>(val.toDouble()));
							file.setFile(obj.value("file").toString());
							file.setFiletype("episode");
							file.setType("episode");
							file.setIcon("episode");
							auto thumbnail = obj.value("thumbnail").toString();
							if (thumbnail.size() > 0)
								file.setThumbnail(getImageUrl(client_, thumbnail).toString());
							val = obj.value("lastplayed");
							file.setPlayed(val.isString() && val.toString().size() > 0);
							val = obj.value("resume");
							episodes.push_back(file);
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
