#include "musicservice.h"
#include "client.h"
#include "settingsmanager.h"
#include "albumsrequest.h"
#include "songsrequest.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

namespace {


int filesPropCount(QQmlListProperty<File>* list)
{
	return static_cast<QList<File*>*>(list->data)->count();
}

File* filesPropAt(QQmlListProperty<File>* list, int index)
{
	auto l = static_cast<QList<File*>*>(list->data);
	if(index < l->size())
		return (*l)[index];
	return nullptr;
}

}

bool MusicService::refreshing() const
{
	return refreshing_;
}

bool MusicService::inputRequested() const
{
	return inputRequested_;
}

QString MusicService::inputTitle() const
{
	return inputTitle_;
}

QString MusicService::inputValue() const
{
	return inputValue_;
}

void MusicService::setRefreshing(bool refreshing)
{
	refreshing_ = refreshing;
	emit refreshingChanged();
}

void MusicService::setInputRequested(bool inputRequested)
{
	if (inputRequested_ == inputRequested)
		return;

	inputRequested_ = inputRequested;
	emit inputRequestedChanged(inputRequested_);
}

void MusicService::setInputTitle(QString inputTitle)
{
	if (inputTitle_ == inputTitle)
		return;

	inputTitle_ = inputTitle;
	emit inputTitleChanged(inputTitle_);
}

void MusicService::setInputValue(QString inputValue)
{
	if (inputValue_ == inputValue)
		return;

	inputValue_ = inputValue;
	emit inputValueChanged(inputValue_);
}

void MusicService::inputBack()
{
	auto message = QJsonRpcMessage::createRequest("Input.Back");
	auto reply = Client::current().send(message);
	connect(reply, &QJsonRpcServiceReply::finished, [this]() { sender()->deleteLater();});
}

void MusicService::inputText(QString inputValue)
{
	QJsonObject parameters;
	parameters["text"] = inputValue;
	parameters["done"] = true;
	auto message = QJsonRpcMessage::createRequest("Input.SendText", parameters);
	auto& client = Client::current();
	auto reply = client.send(message);
	connect(reply, &QJsonRpcServiceReply::finished, [this]() { sender()->deleteLater();});
}

MusicService::MusicService(QObject *parent) :
    QObject(parent),
    browsingMode_(""),
    browsingValue_(""),
    refreshing_(false),
    audioPlaylistId_(0)
{
	connect(&Client::current(), &Client::inputRequested, this, &MusicService::requestInput_);
	connect(&Client::current(), &Client::inputFinished, this, &MusicService::requestInputFinished_);
}

MusicService::MusicService(QString browsingMode, QString browsingValue, QObject* parent) :
    QObject(parent),
    browsingMode_(browsingMode),
    browsingValue_(browsingValue),
    refreshing_(false)
{
}

MusicService::~MusicService()
{
	clearFiles();
}

void MusicService::clearFiles()
{
	/*for(int i = 0; i < files_.count(); ++i)
		delete files_[i];*/
	files_.clear();
}

QList<File *> MusicService::files() const
{
	return files_;
}

QQmlListProperty<File> MusicService::filesAsList()
{
	return QQmlListProperty<File>(this, &files_, &filesPropCount, &filesPropAt);
}

void MusicService::refresh()
{
	clearFiles();
	if(browsingMode_ == "directory")
		refresh_files();
	else if(browsingMode_ == "addon")
		refresh_files();
	else
		refresh_collection();
}

void MusicService::setFiles(const QList<File *> &value)
{
	files_ = value;
}

void MusicService::setBrowsingMode(QString browsingMode)
{
	browsingMode_ = browsingMode;
	emit browsingModeChanged();
}

void MusicService::setBrowsingValue(QString browsingValue)
{
	browsingValue_ = browsingValue;
	emit browsingValueChanged();
}

void MusicService::setLabel(QString label)
{
	label_ = label;
}

QString MusicService::browsingMode() const
{
	return browsingMode_;
}

QString MusicService::browsingValue() const
{
	return browsingValue_;
}

QString MusicService::label() const
{
	return label_;
}

void MusicService::refresh_files()
{
	setRefreshing(true);
	QJsonRpcMessage message;
	if(browsingValue_.size() > 0)
	{
		QJsonObject parameters;
		parameters.insert("directory", browsingValue_);
		if(!browsingValue_.startsWith("addon:") &&
		   !browsingValue_.startsWith("plugin://"))
		{
			QJsonObject obj;
			obj["order"] = QLatin1String("ascending");
			obj["method"] = QLatin1String("label");
			obj["ignorearticle"] = true;
			parameters.insert("sort", obj);
		}
		parameters.insert("media", QString("music"));
		QJsonArray properties;
		properties.push_back("title");
		properties.push_back("artist");
		properties.push_back("track");
		properties.push_back("thumbnail");
		properties.push_back("file");
		parameters.insert("properties", properties);
		message = QJsonRpcMessage::createRequest("Files.GetDirectory", parameters);
	}
	else
	{
		QJsonObject parameters;
		parameters.insert("media", QString::fromLatin1("music"));
		message = QJsonRpcMessage::createRequest("Files.GetSources", parameters);
	}
	QJsonRpcServiceReply* reply = Client::current().send(message);
	if(reply)
		connect(reply, SIGNAL(finished()), this, SLOT(parseDirectoryResults()));
}

void MusicService::refresh_collection()
{
	setRefreshing(true);
	QJsonRpcMessage message;
	if(browsingMode_.size() > 0)
	{
		QJsonObject parameters;
		qDebug() << browsingMode_ << " " << browsingValue_;
		if(browsingMode_ != "addon") // no sort in addon : addons do their own sorting, and it usually messes
		{
			QJsonObject obj;
			obj["order"] = QLatin1String("ascending");
			obj["method"] = QLatin1String("label");
			obj["ignorearticle"] = true;
			parameters.insert("sort", obj);
		}
		QJsonArray properties;
		properties.append(QLatin1String("thumbnail"));
		parameters.insert("properties", properties);
		if(browsingMode_ == "media")
		{
			if(browsingValue_ == "artists")
			{
				message = QJsonRpcMessage::createRequest("AudioLibrary.GetArtists", parameters);
				QJsonRpcServiceReply* reply = Client::current().send(message);
				if(reply)
					connect(reply, SIGNAL(finished()), this, SLOT(parseArtistsResults()));
			}
			else if(browsingMode_ == "media" && browsingValue_ == "albums")
			{
				auto albumRequest = new AlbumsRequest();
				connect(albumRequest, &AlbumsRequest::finished, this, &MusicService::parseAlbumsResults);
				albumRequest->start(0);
			}
			else if(browsingValue_ == "songs")
			{
				auto songsRequest = new SongsRequest();
				connect(songsRequest, &SongsRequest::finished, this, &MusicService::parseSongsResults);
				songsRequest->start(0);
			}
			else if(browsingValue_ == "genres")
			{
				message = QJsonRpcMessage::createRequest("AudioLibrary.GetGenres", parameters);
				QJsonRpcServiceReply* reply = Client::current().send(message);
				if(reply)
					connect(reply, SIGNAL(finished()), this, SLOT(parseGenresResults()));
			}
		}
		else if(browsingMode_ == "artist")
		{
			auto req = new AlbumsRequest();
			connect(req, &AlbumsRequest::finished, this, &MusicService::parseAlbumsResults);
			req->start(browsingValue_.toInt());
		}
		else if(browsingMode_ == "album")
		{
			auto songsRequest = new SongsRequest();
			connect(songsRequest, &SongsRequest::finished, this, &MusicService::parseSongsResults);
			songsRequest->start(browsingValue_.toInt());

		}
		else if(browsingMode_ == "genre")
		{
			auto req = new AlbumsRequest(this);
			connect(req, &AlbumsRequest::finished, this, &MusicService::parseAlbumsResults);
			req->startWithGenre(browsingValue_.toInt());
		}
	}
	else
	{
		files_.clear();
		File* file = new File(this);
		file->setLabel(tr("Artists"));
		file->setFile("artists");
		file->setType("media");
		file->setFiletype("media");
		files_.push_back(file);
		file = new File(this);
		file->setFile("albums");
		file->setLabel(tr("Albums"));
		file->setType("media");
		file->setFiletype("media");
		files_.push_back(file);
		file = new File(this);
		file->setLabel(tr("Songs"));
		file->setFile("songs");
		file->setType("media");
		file->setFiletype("media");
		files_.push_back(file);
		file = new File(this);
		file->setLabel(tr("Genres"));
		file->setFile("genres");
		file->setType("media");
		file->setFiletype("media");
		files_.push_back(file);
		file = new File(this);
		file->setLabel(tr("Files"));
		file->setFile("");
		file->setType("directory");
		file->setFiletype("directory");
		files_.push_back(file);
		refreshAddons_();
		emit filesAsListChanged();
		setRefreshing(false);
	}
}

void MusicService::refreshAddons_()
{
	QJsonObject parameters;
	parameters.insert("type", "xbmc.addon.audio");
	QJsonArray data;
	data.push_back("name");
	data.push_back("thumbnail");
	data.push_back("description");
	data.push_back("summary");
	parameters.insert("properties", data);
	auto message = QJsonRpcMessage::createRequest("Addons.GetAddons", parameters);
	auto reply = Client::current().send(message);
	if(reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &MusicService::parseRefreshAddonsResult_);
	else
	{
		emit filesAsListChanged();
		setRefreshing(false);
	}
}

void MusicService::parseArtistsResults()
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
				files = result.toObject().take("artists");
				if(files.type() == QJsonValue::Array)
				{
					QJsonArray res = files.toArray();
					for(QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
					{
						File* file = new File(this);
						if((*it).type() == QJsonValue::Object)
						{
							QJsonObject obj = (*it).toObject();
							QJsonValue val = obj.value("label");
							if(val.type() == QJsonValue::String)
								file->setLabel(val.toString());
							val = obj.value("artistid");
							if(val.type() == QJsonValue::Double)
								file->setFile(QString::number(val.toDouble()));
							file->setFiletype("artist");
							file->setType("artist");
							file->setThumbnail(getImageUrl(obj.value("thumbnail").toString()).toString());
							files_.push_back(file);
						}
					}
				}
			}
		}
	}
	setRefreshing(false);
	emit filesAsListChanged();
	sender()->deleteLater();
}

void MusicService::parseAlbumsResults()
{
	auto request = dynamic_cast<AlbumsRequest*>(sender());
	if(request)
	{
		files_ = std::move(request->results);
		emit filesAsListChanged();
		request->deleteLater();
	}
	setRefreshing(false);
}

void MusicService::parseSongsResults()
{
	auto request = dynamic_cast<SongsRequest*>(sender());
	if(request)
	{
		files_ = std::move(request->results);
		emit filesAsListChanged();
		request->deleteLater();
	}
	setRefreshing(false);
}

void MusicService::parseGenresResults()
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
				files = result.toObject().take("genres");
				if(files.type() == QJsonValue::Array)
				{
					QJsonArray res = files.toArray();
					for(QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
					{
						File* file = new File(this);
						if((*it).type() == QJsonValue::Object)
						{
							QJsonObject obj = (*it).toObject();
							QJsonValue val = obj.value("label");
							if(val.type() == QJsonValue::String)
								file->setLabel(val.toString());
							val = obj.value("genreid");
							if(val.type() == QJsonValue::Double)
								file->setFile(QString::number(val.toDouble()));
							file->setFiletype("genre");
							file->setType("genre");
							file->setThumbnail(getImageUrl(obj.value("thumbnail").toString()).toString());
							files_.push_back(file);
						}
					}
				}
			}
		}
	}
	setRefreshing(false);
	emit filesAsListChanged();
	sender()->deleteLater();
}

void MusicService::parseDirectoryResults()
{
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	qDebug() << reply;
	if(reply)
	{
		QJsonObject obj = reply->response().toObject();
		if(obj.find("result") != obj.end())
		{
			QJsonValue result = obj.take("result");
			if(result.type() == QJsonValue::Object)
			{
				QJsonValue files;
				if(browsingValue_.size() > 0)
					files = result.toObject().take("files");
				else
					files = result.toObject().take("sources");
				if(files.type() == QJsonValue::Array)
				{
					QJsonArray res = files.toArray();
					for(QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
					{
						File* file = new File(this);
						if((*it).type() == QJsonValue::Object)
						{
							QJsonObject obj = (*it).toObject();
							QJsonValue val = obj.value("file");
							if(val.type() == QJsonValue::String)
								file->setFile(val.toString());
							val = obj.value("filetype");
							if(val.type() == QJsonValue::String)
								file->setFiletype(val.toString());
							else if(browsingValue_.size() == 0)
								file->setFiletype("directory");
							val = obj.value("label");
							if(val.type() == QJsonValue::String)
								file->setLabel(val.toString());
							val = obj.value("type");
							if(val.type() == QJsonValue::String)
								file->setType(val.toString());
							file->setThumbnail(getImageUrl(obj.value("thumbnail").toString()).toString());
							files_.push_back(file);
						}
					}
				}
			}
		}
	}
	setRefreshing(false);
	reply->deleteLater();
	emit filesAsListChanged();
}

void MusicService::parseRefreshAddonsResult_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply)
	{
		auto obj = reply->response().toObject();
		if(obj.find("result") != obj.end())
		{
			QJsonValue result = obj.take("result");
			if(result.type() == QJsonValue::Object)
			{
				auto resobj = result.toObject();
				auto addonsIt = resobj.find("addons");
				if(addonsIt != resobj.end() && addonsIt->isArray())
				{
					for(auto const& addon : addonsIt->toArray())
					{
						if(addon.isObject())
						{
							QJsonObject addonObj = addon.toObject();
							auto file = new File(this);
							file->setLabel(addonObj["name"].toString());
							file->setFile("plugin://" + addonObj["addonid"].toString());
							file->setType("addon");
							file->setFiletype("addon");
							file->setThumbnail(getImageUrl(addonObj["thumbnail"].toString()).toString());
							files_.push_back(file);
						}
					}
				}
			}
		}
	}
	emit filesAsListChanged();
	setRefreshing(false);
}

void MusicService::requestInput_(QString title, QString type, QString value)
{
	setInputTitle(title);
	setInputValue(value);
	setInputRequested(true);
}

void MusicService::requestInputFinished_()
{
	setInputRequested(false);
}

}
}
}
