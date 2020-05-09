#include "tvshowseasonsrequest.h"

#include "client.h"
#include "file.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

TvShowSeasonsRequest::TvShowSeasonsRequest(Client* client, QObject* parent) :
    QObject(parent), client_{client}, success(false)
{
}

TvShowSeasonsRequest::~TvShowSeasonsRequest()
{
	seasons.clear();
}

void TvShowSeasonsRequest::start(int tvshowid)
{
	QJsonObject parameters;
	parameters.insert("tvshowid", tvshowid);
	QJsonArray properties;
	properties.push_back(QString("season"));
	properties.push_back(QString::fromUtf8("thumbnail"));
	parameters.insert("properties", properties);
	QJsonObject sort;
	sort.insert("order", QLatin1String("ascending"));
	sort.insert("method", QLatin1String("season"));
	parameters.insert("sort", sort);
	auto message = QJsonRpcMessage::createRequest("VideoLibrary.GetSeasons", parameters);
	QJsonRpcServiceReply* reply = client_->send(message);
	if (reply)
		connect(
		    reply, &QJsonRpcServiceReply::finished, this, [tvshowid, this]() { this->parseSeasonsResult_(tvshowid); });
	else
		emit finished();
}

void TvShowSeasonsRequest::parseSeasonsResult_(int tvshowid)
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
				files = result.toObject().take("seasons");
				if (files.type() == QJsonValue::Array)
				{
					QJsonArray const res = files.toArray();
					for (auto const& f : res)
					{
						if (f.type() == QJsonValue::Object)
						{
							File file;
							QJsonObject obj = f.toObject();
							QJsonValue val = obj.value("label");
							if (val.type() == QJsonValue::String)
								file.setLabel(val.toString());
							val = obj.value("season");
							if (val.type() == QJsonValue::Double)
								file.setFile(QString::number(tvshowid) + "|" + QString::number(val.toDouble()));
							file.setFiletype("season");
							file.setType("season");
							auto const thumbnail = obj.value("thumbnail").toString();
							if(thumbnail.size() > 0)
								file.setThumbnail(getImageUrl(client_, thumbnail).toString());
							this->seasons.push_back(file);
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
