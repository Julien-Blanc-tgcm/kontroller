#include "imageservice.h"

#include "../utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

bool ImageService::refreshing() const
{
	return refreshing_;
}

bool ImageService::inputRequested() const
{
	return inputRequested_;
}

QString ImageService::inputTitle() const
{
	return inputTitle_;
}

QString ImageService::inputValue() const
{
	return inputValue_;
}

Client* ImageService::client() const
{
	return client_;
}

bool ImageService::alwaysDisplayIcons() const
{
	return browsingMode_ == "directory";
}

void ImageService::setRefreshing(bool refreshing)
{
	refreshing_ = refreshing;
	emit refreshingChanged();
}

void ImageService::setInputRequested(bool inputRequested)
{
	if (inputRequested_ == inputRequested)
		return;

	inputRequested_ = inputRequested;
	emit inputRequestedChanged(inputRequested_);
}

void ImageService::setInputTitle(QString inputTitle)
{
	if (inputTitle_ == inputTitle)
		return;

	inputTitle_ = inputTitle;
	emit inputTitleChanged(inputTitle_);
}

void ImageService::setInputValue(QString inputValue)
{
	if (inputValue_ == inputValue)
		return;

	inputValue_ = inputValue;
	emit inputValueChanged(inputValue_);
}

void ImageService::inputBack()
{
	auto message = QJsonRpcMessage::createRequest("Input.Back");
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, [this]() { sender()->deleteLater();});
}

void ImageService::inputText(QString inputValue)
{
	QJsonObject parameters;
	parameters["text"] = inputValue;
	parameters["done"] = true;
	auto message = QJsonRpcMessage::createRequest("Input.SendText", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, [this]() { sender()->deleteLater();});
}

void ImageService::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	connect(client_, &Client::inputRequested, this, &ImageService::requestInput_);
	connect(client_, &Client::inputFinished, this, &ImageService::requestInputFinished_);
	emit clientChanged(client_);
}

ImageService::ImageService(QObject *parent) :
    QObject(parent)
{
}

ImageService::ImageService(QString browsingMode, QString browsingValue, QObject* parent) :
    QObject(parent),
    browsingMode_(browsingMode),
    browsingValue_(browsingValue)
{
}

ImageService::~ImageService() noexcept
{
	clearFiles();
}

void ImageService::clearFiles()
{
	files_.clear();
}

QVector<File> ImageService::files() const
{
	return files_;
}

QVariantList ImageService::filesAsList()
{
	QVariantList l;
	for(auto f : files_)
		l.push_back(QVariant::fromValue(f));
	return l;
}

void ImageService::refresh()
{
	clearFiles();
	if(browsingMode_ == "directory")
		refresh_files();
	else if(browsingMode_ == "addon")
		refresh_files();
	else
	{
		refresh_files();
		refreshAddons_(); // also add addons at root
	}
}

void ImageService::setFiles(const QVector<File> &value)
{
	files_ = value;
}

void ImageService::setBrowsingMode(QString browsingMode)
{
	browsingMode_ = browsingMode;
	emit browsingModeChanged();
}

void ImageService::setBrowsingValue(QString browsingValue)
{
	browsingValue_ = browsingValue;
	emit browsingValueChanged();
}

QString ImageService::browsingMode() const
{
	return browsingMode_;
}

QString ImageService::browsingValue() const
{
	return browsingValue_;
}

void ImageService::refresh_files()
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
			obj["method"] = QLatin1String("file");
			obj["ignorearticle"] = client_->sortIgnoreArticle();
			parameters.insert("sort", obj);
		}
		parameters.insert("media", QString("pictures"));
		QJsonArray properties;
		properties.push_back("thumbnail");
		properties.push_back("file");
		parameters.insert("properties", properties);
		message = QJsonRpcMessage::createRequest("Files.GetDirectory", parameters);
	}
	else
	{
		QJsonObject parameters;
		parameters.insert("media", QString::fromLatin1("pictures"));
		message = QJsonRpcMessage::createRequest("Files.GetSources", parameters);
	}
	QJsonRpcServiceReply* reply = client_->send(message);
	if(reply)
		connect(reply, SIGNAL(finished()), this, SLOT(parseDirectoryResults()));
}

void ImageService::refreshAddons_()
{
	QJsonObject parameters;
	parameters.insert("type", "xbmc.addon.image");
	QJsonArray data;
	data.push_back("name");
	data.push_back("thumbnail");
	data.push_back("description");
	data.push_back("summary");
	parameters.insert("properties", data);
	auto message = QJsonRpcMessage::createRequest("Addons.GetAddons", parameters);
	auto reply = client_->send(message);
	if(reply)
		connect(reply, &QJsonRpcServiceReply::finished, this, &ImageService::parseRefreshAddonsResult_);
	else
	{
		emit filesAsListChanged();
		setRefreshing(false);
	}
}

void ImageService::parseDirectoryResults()
{
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
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
					QJsonArray const res = files.toArray();
					for(QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
					{
						File file;
						if((*it).type() == QJsonValue::Object)
						{
							QJsonObject obj = (*it).toObject();
							QJsonValue val = obj.value("file");
							if(val.type() == QJsonValue::String)
								file.setFile(val.toString());
							val = obj.value("filetype");
							if(val.type() == QJsonValue::String)
								file.setFiletype(val.toString());
							else if(browsingValue_.size() == 0)
								file.setFiletype("directory");
							val = obj.value("label");
							if(val.type() == QJsonValue::String)
								file.setLabel(val.toString());
							val = obj.value("type");
							if(val.type() == QJsonValue::String)
								file.setType(val.toString());
							auto thumbnail = obj.value("thumbnail").toString();
							if (thumbnail.size() == 0 && file.filetype() == "file")
								thumbnail = "image://" + QUrl::toPercentEncoding(file.file()) + "/transform?size=thumb";
							qDebug() << thumbnail << file.filetype();
							file.setThumbnail(getImageUrl(client_, thumbnail).toString());

							if (file.filetype() == "directory")
								file.setIcon("folder");
							else if (file.filetype() == "file")
								file.setIcon("image");
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

void ImageService::parseRefreshAddonsResult_()
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
				auto const resobj = result.toObject();
				auto const addonsIt = resobj.find("addons");
				if(addonsIt != resobj.end() && addonsIt->isArray())
				{
					auto const addonsArray = addonsIt->toArray();
					for(auto const& addon : addonsArray)
					{
						if(addon.isObject())
						{
							QJsonObject addonObj = addon.toObject();
							File file;
							file.setLabel(addonObj["name"].toString());
							file.setFile("plugin://" + addonObj["addonid"].toString());
							file.setType("addon");
							file.setFiletype("addon");
							file.setIcon("addon");
							file.setThumbnail(getImageUrl(client_, addonObj["thumbnail"].toString()).toString());
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

void ImageService::requestInput_(QString title, QString /*type*/, QString value)
{
	setInputTitle(title);
	setInputValue(value);
	setInputRequested(true);
}

void ImageService::requestInputFinished_()
{
	setInputRequested(false);
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
