#include "downloadservice.h"

#include "applicationsettings.h"
#include "client.h"
#include "file.h"
#include "requests/songsrequest.h"

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

DownloadService::DownloadService(Client* client, ApplicationSettings* settings, QObject* parent) :
    QObject(parent), client_{client}, appSettings_{settings}
{
}

void DownloadService::addFile(QString filePath, QString mediaType)
{
	addFile_(filePath, getRelevantFolder_(mediaType));
}

void DownloadService::addFile_(QString filePath, QString outputFolder)
{
	queue_.push_back(FileDownload{FileType::File, filePath, outputFolder, 0, nullptr, nullptr});
	if (queue_.size() == 1) // just added a file
	{
		startNextDownload_();
	}
}

void DownloadService::addFolder(QString filePath, QString mediaType)
{
	addFolder_(filePath, getRelevantFolder_(mediaType));
}

void DownloadService::addAlbum(QString albumName, int albumId)
{
	queue_.push_back(
	    FileDownload{FileType::Album, albumName, getRelevantFolder_("music"), albumId, nullptr, nullptr});
	if (queue_.size() == 1)
		startNextDownload_();
}

void DownloadService::addTvShow(QString tvShowName, int tvShowId)
{
	queue_.push_back(
	    FileDownload{FileType::Album, tvShowName, getRelevantFolder_("video"), tvShowId, nullptr, nullptr});
	if (queue_.size() == 1)
		startNextDownload_();
}


void DownloadService::addFolder_(QString filePath, QString outputFolder)
{
	queue_.push_back(FileDownload{FileType::Directory, filePath, outputFolder, 0, nullptr, nullptr});
	if (queue_.size() == 1) // just added a file
	{
		startNextDownload_();
	}
}

QString DownloadService::getRelevantFolder_(QString mediaType)
{
	if (mediaType == "video")
	{
		auto list = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation);
		if (list.size() > 0)
			return appSettings_->downloadLocation().baseFolder() + "/" + QFileInfo(list.first()).fileName();
		return appSettings_->downloadLocation().baseFolder();
	}
	else if (mediaType == "music")
	{
		auto list = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
		if (list.size() > 0)
			return appSettings_->downloadLocation().baseFolder() + "/" + QFileInfo(list.first()).fileName();
		return appSettings_->downloadLocation().baseFolder();
	}
	return appSettings_->downloadLocation().baseFolder();
}

void DownloadService::filePathRequestComplete_()
{
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		reply->deleteLater();
		auto response = reply->response();
		auto obj = response.toObject();
		if (obj.find("result") != obj.end())
		{
			auto result = obj.take("result");
			if (result.type() == QJsonValue::Object)
			{
				QString finalPath;
				auto details = result.toObject().take("details");
				if (details.type() == QJsonValue::Object)
				{
					auto path = details.toObject().take("path");
					if (path.type() != QJsonValue::String)
					{
						emit downloadError("", "Invalid path value");
						return;
					}
					finalPath = path.toString();
				}
				auto protocol = result.toObject().take("protocol");
				if (protocol.type() != QJsonValue::String || protocol.toString() != "http")
				{
					emit downloadError("", "Protocol not supported or invalid");
					return;
				}
				auto mode = result.toObject().take("mode");
				if (mode.type() != QJsonValue::String || mode.toString() != "redirect")
				{
					emit downloadError("", "Mode not supported or invalid");
					return;
				}
				startDownloadHttp_(finalPath);
			}
			else
				emit downloadError("", "Invalid reply received");
		}
	}
	else
		emit downloadError("", "Invalid reply received");
}

void DownloadService::downloadCompleted_()
{
	auto reply = dynamic_cast<QNetworkReply*>(sender());
	if (reply)
	{
		reply->deleteLater();
		emit downloadCompleted("");
	}
	QString file = queue_.front().sourceFile;
	QFileInfo info(file);
	emit downloadCompleted(info.fileName());
	queue_.erase(queue_.begin());
	if (queue_.size() > 0)
	{
		startNextDownload_();
	}
}

void DownloadService::startNextDownload_()
{
	auto& file = queue_.front();
	if (file.type == FileType::File)
	{
		QJsonObject parameters;
		parameters.insert("path", file.sourceFile);
		auto message = QJsonRpcMessage::createRequest("Files.PrepareDownload", parameters);
		auto reply = client_->send(message);
		if (reply)
			connect(reply, &QJsonRpcServiceReply::finished, this, &DownloadService::filePathRequestComplete_);
		else
		{
			QFileInfo info(queue_.front().sourceFile);
			emit downloadError(info.fileName(), "Query failed");
			if (queue_.front().output != nullptr)
				queue_.front().output->deleteLater();
			queue_.erase(queue_.begin());
			if (queue_.size() != 0)
				startNextDownload_();
		}
	}
	else if (file.type == FileType::Directory)
	{
		// query directory for its files
		QJsonObject parameters;
		parameters.insert("directory", file.sourceFile);
		QString sourceFile = file.sourceFile;
		if (sourceFile.endsWith('/'))
			sourceFile = sourceFile.left(sourceFile.length() - 1); // remove trailing /
		QFileInfo info(sourceFile);
		QString name = info.fileName();
		queue_.front().destinationPath += QDir::separator() + name;
		QJsonArray properties;
		properties.push_back("file");
		parameters.insert("properties", properties);
		auto message = QJsonRpcMessage::createRequest("Files.GetDirectory", parameters);
		QJsonRpcServiceReply* reply = client_->send(message);
		if (reply)
		{
			connect(reply, SIGNAL(finished()), this, SLOT(folderInfoRequestComplete_()));
		}
		else
		{
			emit downloadError(info.fileName(), "Query failed");
			if (queue_.front().output != nullptr)
				queue_.front().output->deleteLater();
			queue_.erase(queue_.begin());
			if (queue_.size() != 0)
				startNextDownload_();
		}
	}
	else if (file.type == FileType::Album)
	{ // query album for its files
		auto req = new SongsRequest(client_, this);
		connect(req, &SongsRequest::finished, this, &DownloadService::albumInfoReceived_);
		req->start(file.id);
	}
	else
	{
		queue_.erase(queue_.begin());
		if (queue_.size() != 0)
			startNextDownload_();
	}
}

void DownloadService::bytesAvailable_()
{
	auto& file = queue_.front();
	file.output->write(file.reply->readAll());
}

void DownloadService::folderInfoRequestComplete_()
{
	QJsonRpcServiceReply* reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		reply->deleteLater();
		auto response = reply->response();
		auto obj = response.toObject();
		if (obj.find("result") != obj.end())
		{
			auto result = obj.take("result");
			if (result.type() == QJsonValue::Object)
			{
				QJsonValue files = result.toObject().take("files");
				if (files.type() == QJsonValue::Array)
				{
					QJsonArray res = files.toArray();
					for (QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
					{
						if (it->type() == QJsonValue::Object)
						{
							QString file;
							QJsonObject obj = it->toObject();
							QJsonValue val = obj.value("file");
							if (val.type() == QJsonValue::String)
								file = val.toString();
							val = obj.value("filetype");
							if (val.type() == QJsonValue::String)
							{
								if (val.toString() == "file")
									addFile_(file,
									         queue_.front().destinationPath + QDir::separator() +
									             QFileInfo(queue_.front().sourceFile).fileName());
								else if (val.toString() == "directory")
									addFolder_(file,
									           queue_.front().destinationPath + QDir::separator() +
									               QFileInfo(queue_.front().sourceFile).fileName());
							}
						}
					}
				}
			}
		}
	}
	else
		emit downloadError("", "Invalid reply received");
	queue_.front().output->deleteLater();
	queue_.erase(queue_.begin());
	if (queue_.size() > 0)
		startNextDownload_();
}

void DownloadService::albumInfoReceived_()
{
	auto req = dynamic_cast<SongsRequest*>(sender());
	if (req != nullptr)
	{
		if (!req->success)
		{
			emit downloadError("Album", "Failed");
		}
		else
		{
			auto const songs = req->results;
			for (auto const& song : songs)
			{
				addFile_(song.file(), queue_.front().destinationPath + QDir::separator() + queue_.front().sourceFile);
			}
		}
	}
	queue_.erase(queue_.begin());
	if (queue_.size() > 0)
		startNextDownload_();
}

void DownloadService::startDownloadHttp_(QString httppath)
{
	auto& file = queue_.front();
	file.reply = client_->downloadFile(httppath);
	QFileInfo source(file.sourceFile);
	QFileInfo dest(file.destinationPath + QDir::separator() + source.fileName());
	QDir dir(dest.absoluteDir());
	if (!dir.exists())
		QDir("/").mkpath(dir.absolutePath());
	file.output = new QFile(dest.absoluteFilePath());
	file.output->open(QFile::WriteOnly);
	connect(file.reply, &QNetworkReply::finished, this, &DownloadService::downloadCompleted_);
	connect(file.reply, &QNetworkReply::downloadProgress, this, &DownloadService::bytesAvailable_);
	emit downloadStarted(source.fileName());
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
