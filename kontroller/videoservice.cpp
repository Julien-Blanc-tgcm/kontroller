#include "videoservice.h"
#include "client.h"
#include "tvshowepisodesrequest.h"
#include "tvshowseasonsrequest.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

bool VideoService::refreshing() const
{
	return refreshing_;
}

bool VideoService::inputRequested() const
{
	return inputRequested_;
}

QString VideoService::inputTitle() const
{
	return inputTitle_;
}

QString VideoService::inputValue() const
{
	return inputValue_;
}

Client* VideoService::client() const
{
	return client_;
}

void VideoService::setRefreshing(bool refreshing)
{
	refreshing_ = refreshing;
	emit refreshingChanged();
}

VideoService::VideoService(QObject* parent) :
    QObject(parent), browsingMode_(""), browsingValue_(""), refreshing_(false), inputRequested_{false}
{
}

VideoService::~VideoService()
{
	clearFiles();
}

void VideoService::clearFiles()
{
	/*for(int i = 0; i < files_.count(); ++i)
	    delete files_[i];*/
	files_.clear();
}

QVector<File> VideoService::files() const
{
	return files_;
}

QVariantList VideoService::filesAsList() const
{
	QVariantList l;
	for (auto const& f : files_)
		l.push_back(QVariant::fromValue(f));
	return l;
}

void VideoService::refresh()
{
	clearFiles();
	if (browsingMode_ == "directory")
		refresh_files();
	else
		refresh_collection();
}

void VideoService::refreshCollection()
{
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("VideoLibrary.Scan");
	client_->send(message);
}

void VideoService::setInputRequested(bool inputRequested)
{
	if (inputRequested_ == inputRequested)
		return;

	inputRequested_ = inputRequested;
	emit inputRequestedChanged(inputRequested_);
}

void VideoService::setInputTitle(QString inputTitle)
{
	if (inputTitle_ == inputTitle)
		return;

	inputTitle_ = inputTitle;
	emit inputTitleChanged(inputTitle_);
}

void VideoService::setInputValue(QString inputValue)
{
	if (inputValue_ == inputValue)
		return;

	inputValue_ = inputValue;
	emit inputValueChanged(inputValue_);
}

void VideoService::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	connect(client_, &Client::inputRequested, this, &VideoService::requestInput_);
	connect(client_, &Client::inputFinished, this, &VideoService::requestInputFinished_);
	emit clientChanged(client_);
}

void VideoService::setFiles(const QVector<File>& value)
{
	files_ = value;
	emit filesChanged();
}

void VideoService::setBrowsingMode(QString browsingMode)
{
	browsingMode_ = browsingMode;
	emit browsingModeChanged();
}

void VideoService::setBrowsingValue(QString browsingValue)
{
	browsingValue_ = browsingValue;
	emit browsingValueChanged();
}

QString VideoService::browsingMode() const
{
	return browsingMode_;
}

QString VideoService::browsingValue() const
{
	return browsingValue_;
}

void VideoService::refresh_files()
{
	setRefreshing(true);
	QJsonRpcMessage message;
	if (browsingValue_.size() > 0)
	{
		QJsonObject parameters;
		parameters.insert("directory", browsingValue_);
		QJsonObject obj;
		obj["order"] = QLatin1String("ascending");
		obj["method"] = QLatin1String("file");
		obj["ignorearticle"] = client_->sortIgnoreArticle();
		parameters.insert("media", QLatin1String("video"));
		parameters.insert("sort", obj);
		QJsonArray properties;
		properties.push_back("thumbnail");
		parameters.insert("properties", properties);
		message = QJsonRpcMessage::createRequest("Files.GetDirectory", parameters);
	}
	else
	{
		QJsonObject parameters;
		parameters.insert("media", QString::fromLatin1("video"));
		message = QJsonRpcMessage::createRequest("Files.GetSources", parameters);
	}
	QJsonRpcServiceReply* reply = client_->send(message);
	if (reply)
		connect(reply, SIGNAL(finished()), this, SLOT(parseDirectoryResults()));
}

void VideoService::refresh_collection()
{
	setRefreshing(true);
	QJsonRpcMessage message;
	if (browsingMode_.size() > 0)
	{
		QJsonObject obj;
		obj["order"] = QLatin1String("ascending");
		obj["method"] = QLatin1String("label");
		obj["ignorearticle"] = client_->sortIgnoreArticle();
		QJsonObject parameters;
		parameters.insert("sort", obj);
		QJsonArray properties;
		properties.append(QLatin1String("thumbnail"));
		parameters.insert("properties", properties);
		if (browsingMode_ == "media")
		{
			if (browsingValue_ == "movies")
			{
				message = QJsonRpcMessage::createRequest("VideoLibrary.GetMovies", parameters);
				QJsonRpcServiceReply* reply = client_->send(message);
				if (reply)
					connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseMoviesResults_);
			}
			else if (browsingValue_ == "tvshows")
			{
				message = QJsonRpcMessage::createRequest("VideoLibrary.GetTVShows", parameters);
				QJsonRpcServiceReply* reply = client_->send(message);
				if (reply)
					connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseTVShowsResults_);
			}
			else if (browsingValue_ == "musicvideos")
			{
				message = QJsonRpcMessage::createRequest("VideoLibrary.GetMusicVideos", parameters);
				QJsonRpcServiceReply* reply = client_->send(message);
				if (reply)
					connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseMusicVideosResults_);
			}
			else if (browsingValue_ == "genres")
			{
				message = QJsonRpcMessage::createRequest("VideoLibrary.GetGenres", parameters);
				QJsonRpcServiceReply* reply = client_->send(message);
				if (reply)
					connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseGenresResults_);
			}
		}
		else if (browsingMode_ == "tvshow")
		{
			auto req = new TvShowSeasonsRequest(client_, this);
			connect(req, &TvShowSeasonsRequest::finished, this, &VideoService::parseSeasonsResults_);
			req->start(browsingValue_.toInt());
		}
		else if (browsingMode_ == "season")
		{
			QStringList list = browsingValue_.split('|');
			if (list.size() != 2)
			{
				setRefreshing(false);
				return;
			}
			auto req = new TvShowEpisodesRequest(client_);
			connect(req, &TvShowEpisodesRequest::finished, this, &VideoService::parseEpisodesResults_);
			req->start(list[0].toInt(), list[1].toInt());
		}
		else if (browsingMode_ == "album")
		{
			QJsonObject filter;
			filter["albumid"] = browsingValue_.toInt();
			parameters.insert("filter", filter);
			message = QJsonRpcMessage::createRequest("AudioLibrary.GetSongs", parameters);
			QJsonRpcServiceReply* reply = client_->send(message);
			if (reply)
				connect(reply, SIGNAL(finished()), this, SLOT(parseSongsResults()));
		}
		else if (browsingMode_ == "genre")
		{
			QJsonObject filter;
			filter["genreid"] = browsingValue_.toInt();
			parameters.insert("filter", filter);
			message = QJsonRpcMessage::createRequest("AudioLibrary.GetAlbums", parameters);
			QJsonRpcServiceReply* reply = client_->send(message);
			if (reply)
				connect(reply, SIGNAL(finished()), this, SLOT(parseAlbumsResults()));
		}
		else
			setRefreshing(false);
	}
	else
	{
		File file;
		file.setLabel(tr("Movies"));
		file.setFile("movies");
		file.setType("media");
		file.setFiletype("media");
		file.setIcon("movies");
		files_.push_back(file);
		file = File();
		file.setLabel(tr("TV Shows"));
		file.setFile("tvshows");
		file.setType("media");
		file.setFiletype("media");
		file.setIcon("tvshows");
		files_.push_back(file);
		file = File();
		file.setLabel(tr("Clips"));
		file.setFile("musicvideos");
		file.setType("media");
		file.setFiletype("media");
		file.setIcon("clips");
		files_.push_back(file);
		/*        file = File();
		        file.setLabel(tr("Genres"));
		        file.setFile("genres");
		        file.setType("media");
		        file.setFiletype("media");
		        files_.push_back(file); */
		file = File();
		file.setLabel(tr("Files"));
		file.setFile("");
		file.setType("directory");
		file.setFiletype("directory");
		file.setIcon("folder");
		files_.push_back(file);
		emit filesChanged();
		setRefreshing(false);
	}
}

void VideoService::parseMoviesResults_()
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
							file.setThumbnail(getImageUrl(client_, obj.value("thumbnail").toString()).toString());
							files_.push_back(file);
						}
					}
				}
			}
		}
	}
	setRefreshing(false);
	emit filesChanged();
	sender()->deleteLater();
}

void VideoService::parseTVShowsResults_()
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
							file.setThumbnail(getImageUrl(client_, obj.value("thumbnail").toString()).toString());
							files_.push_back(file);
						}
					}
				}
			}
		}
	}
	emit filesChanged();
	setRefreshing(false);
	sender()->deleteLater();
}

void VideoService::parseMusicVideosResults_()
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
				files = result.toObject().take("musicvideos");
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
							val = obj.value("musicvideoid");
							if (val.type() == QJsonValue::Double)
								file.setId(static_cast<int>(val.toDouble()));
							file.setFile(obj.value("file").toString());
							file.setFiletype("musicvideo");
							file.setType("musicvideo");
							file.setThumbnail(getImageUrl(client_, obj.value("thumbnail").toString()).toString());
							files_.push_back(file);
						}
					}
				}
			}
		}
	}
	setRefreshing(false);
	emit filesChanged();
	sender()->deleteLater();
}

void VideoService::parseGenresResults_()
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
				files = result.toObject().take("genres");
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
							val = obj.value("genreid");
							if (val.type() == QJsonValue::Double)
								file.setFile(QString::number(val.toDouble()));
							file.setFiletype("genre");
							file.setType("genre");
							files_.push_back(file);
						}
					}
				}
			}
		}
	}
	setRefreshing(false);
	emit filesChanged();
	sender()->deleteLater();
}

void VideoService::parseSeasonsResults_()
{
	auto reply = dynamic_cast<TvShowSeasonsRequest*>(sender());
	if (reply)
	{
		files_ = reply->seasons;
	}
	setRefreshing(false);
	emit filesChanged();
	sender()->deleteLater();
}

void VideoService::parseEpisodesResults_()
{
	auto reply = dynamic_cast<TvShowEpisodesRequest*>(sender());
	if (reply)
	{
		files_ = reply->episodes;
		reply->episodes.clear();
	}
	setRefreshing(false);
	emit filesChanged();
	sender()->deleteLater();
}

void VideoService::parseDirectoryResults()
{
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		QJsonObject obj = reply->response().toObject();
		if (obj.find("result") != obj.end())
		{
			QJsonValue result = obj.take("result");
			if (result.type() == QJsonValue::Object)
			{
				QJsonValue files;
				if (browsingValue_.size() > 0)
					files = result.toObject().take("files");
				else
					files = result.toObject().take("sources");
				if (files.type() == QJsonValue::Array)
				{
					QJsonArray res = files.toArray();
					for (QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
					{
						File file;
						if ((*it).type() == QJsonValue::Object)
						{
							QJsonObject obj = (*it).toObject();
							QJsonValue val = obj.value("file");
							if (val.type() == QJsonValue::String)
								file.setFile(val.toString());
							val = obj.value("filetype");
							if (val.type() == QJsonValue::String)
								file.setFiletype(val.toString());
							else if (browsingValue_.size() == 0)
								file.setFiletype("directory");
							val = obj.value("label");
							if (val.type() == QJsonValue::String)
								file.setLabel(val.toString());
							val = obj.value("type");
							if (val.type() == QJsonValue::String)
								file.setType(val.toString());
							if (file.filetype() == "directory")
								file.setIcon("folder");
							else
								file.setIcon("video");
							file.setThumbnail(getImageUrl(client_, obj.value("thumbnail").toString()).toString());
							files_.push_back(file);
						}
					}
				}
			}
		}
	}
	setRefreshing(false);
	reply->deleteLater();
	emit filesChanged();
}

void VideoService::requestInput_(QString title, QString /*type*/, QString value)
{
	setInputTitle(title);
	setInputValue(value);
	setInputRequested(true);
}

void VideoService::requestInputFinished_()
{
	setInputRequested(false);
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
