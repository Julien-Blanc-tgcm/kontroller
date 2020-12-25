#include "videocontrol.h"

#include "client.h"
#include "file.h"
namespace eu
{
namespace tgcm
{
namespace kontroller
{

VideoControl::VideoControl(QObject *parent) : QObject(parent)
{

}

Client* VideoControl::client() const
{
	return client_;
}

void VideoControl::playFile(File file)
{
	auto reply = clearPlaylist();
	currentFile_ = file;
	connect(reply,
	        &QJsonRpcServiceReply::finished,
	        this,
	        &VideoControl::addCurrentFileToPlaylist_);

}

void VideoControl::resumeFile(File file, int position)
{
	QJsonRpcMessage message;
	QJsonObject params;
	QJsonObject item;
	if (file.filetype() == "directory")
		item.insert("directory", file.file());
	else if (file.filetype() == "file")
		item.insert("file", file.file());
	else if (file.filetype() == "movie")
		item.insert("movieid", file.id());
	else if (file.filetype() == "tvshow")
		item.insert("tvshowid", file.id());
	else if (file.filetype() == "episode")
		item.insert("episodeid", file.id());
	else if (file.filetype() == "musicvideo")
		item.insert("musicvideoid", file.id());
	params.insert("item", item);
	if (position != 0)
	{
		QJsonObject options;
		QJsonObject timeResume;
		int hours = position / 3600;
		timeResume.insert("hours", hours);
		int remaining = position % 3600;
		timeResume.insert("minutes", remaining / 60);
		remaining = remaining % 60;
		timeResume.insert("seconds", remaining);
		timeResume.insert("milliseconds", 0);
		options.insert("resume", timeResume);
		params.insert("options", options);
	}
	message = QJsonRpcMessage::createRequest("Player.Open", params);
	client_->send(message);
}

QJsonRpcServiceReply* VideoControl::clearPlaylist()
{
	QJsonRpcMessage message;
	QJsonObject params;
	params.insert("playlistid", videoPlaylistId_);
	message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
	return client_->send(message);
}

QJsonRpcServiceReply* VideoControl::addToPlaylist(File file)
{
	QJsonRpcMessage message;
	QJsonObject params;
	QJsonObject item;
	if(file.filetype() == "directory")
		item.insert("directory", file.file());
	else if(file.filetype() == "file")
		item.insert("file", file.file());
	else if(file.filetype() == "movie")
		item.insert("movieid", file.id());
	else if(file.filetype() == "tvshow")
		item.insert("tvshowid", file.id());
	else if(file.filetype() == "episode")
		item.insert("episodeid", file.id());
	else if(file.filetype() == "musicvideo")
		item.insert("musicvideoid", file.id());
	params.insert("item", item);
	params.insert("playlistid", videoPlaylistId_);
	message = QJsonRpcMessage::createRequest("Playlist.Add", params);
	return client_->send(message);
}

void VideoControl::startPlaying()
{
	QJsonRpcMessage message;
	QJsonObject params;
	QJsonObject item;
	item.insert("playlistid", videoPlaylistId_);
	params.insert("item", item);
	message = QJsonRpcMessage::createRequest("Player.Open", params);
	client_->send(message);
}

void VideoControl::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	emit clientChanged(client_);
}

void VideoControl::addCurrentFileToPlaylist_()
{
	auto ret = addToPlaylist(currentFile_);
	connect(ret, &QJsonRpcServiceReply::finished, this, &VideoControl::startPlaying);
}

}
}
}
