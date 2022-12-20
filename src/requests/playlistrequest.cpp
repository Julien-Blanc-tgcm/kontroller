#include "playlistrequest.hpp"

#include "../utils.h"

namespace eu::tgcm::kontroller
{

PlaylistRequest::PlaylistRequest(Client* client, QObject* parent) : QObject(parent), client_{client}
{
}

PlaylistRequest::~PlaylistRequest() noexcept = default;

void PlaylistRequest::start()
{
	QJsonObject parameters;
	parameters["directory"] = "special://musicplaylists";
	auto message = QJsonRpcMessage::createRequest("Files.GetDirectory", parameters);
	QJsonRpcServiceReply* reply = client_->send(message);
	if (reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &PlaylistRequest::parsePlaylistsResult_);
	else
		emit finished();
}

void PlaylistRequest::startWithPlaylist(QString playlist)
{
	QJsonObject parameters;
	parameters["directory"] = playlist;
	auto message = QJsonRpcMessage::createRequest("Files.GetDirectory", parameters);
	QJsonRpcServiceReply* reply = client_->send(message);
	if (reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &PlaylistRequest::parsePlaylistsResult_);
	else
		emit finished();
}

void PlaylistRequest::parsePlaylistsResult_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		qDebug() << reply->response();
		results.clear();
		QJsonRpcMessage response = reply->response();
		QJsonObject obj = response.toObject();
		if (obj.find("result") != obj.end())
		{
			QJsonValue result = obj.value("result");
			if (result.type() == QJsonValue::Object)
			{
				QJsonValue files;
				files = result.toObject().value("files");
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
							val = obj.value("file");
							if (val.type() == QJsonValue::Double)
								file.setId(static_cast<int>(val.toDouble()));
							file.setFile(obj.value("file").toString());
							if (obj.value("filetype").toString() == "directory")
							{
								file.setFiletype("playlist");
								file.setType("playlist");
								file.setIcon("playlists");
							}
							else
							{
								file.setFiletype(obj.value("filetype").toString());
								file.setThumbnail(getImageUrl(client_, obj.value("thumbnail").toString()).toString());
								file.setIcon("music");
							}
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

} // namespace eu::tgcm::kontroller
