#include "imagecontrol.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

ImageControl::ImageControl(QObject* parent) : QObject(parent)
{
}

Client* ImageControl::client() const
{
	return client_;
}

void ImageControl::setClient(Client* client)
{
	if (client_ == client)
		return;
	client_ = client;
	emit clientChanged();
}

void ImageControl::playFile(File file)
{
	if (file.filetype() == "file")
	{
		QJsonObject params;
		QJsonObject item;
		item.insert("file", file.file());
		params.insert("item", item);
		params.insert("options", QJsonObject{});
		auto message = QJsonRpcMessage::createRequest("Player.Open", params);
		client_->send(message);
	}
	else if (file.filetype() == "directory")
	{
		QJsonObject params;
		QJsonObject item;
		item.insert("path", file.file());
		params.insert("item", item);
		params.insert("options", QJsonObject{});
		auto message = QJsonRpcMessage::createRequest("Player.Open", params);
		client_->send(message);
	}
	else
		qDebug() << "Unsupported file type: " << file.filetype();
}

void ImageControl::addToPlaylist(File file)
{
	QJsonRpcMessage message;
	QJsonObject params;
	QJsonObject item;
	if(file.filetype() == "directory")
		item.insert("directory", file.file());
	else if(file.filetype() == "file")
		item.insert("file", file.file());
	else if(file.filetype() == "album")
		item.insert("albumid", file.id());
	else if(file.filetype() == "song")
		item.insert("songid", file.id());
	params.insert("item", item);
	params.insert("playlistid", imagePlaylistId_);
	message = QJsonRpcMessage::createRequest("Playlist.Add", params);
	client_->send(message);
}

void ImageControl::startPlaying()
{
	QJsonRpcMessage message;
	QJsonObject params;
	QJsonObject item;
	item.insert("playlistid", imagePlaylistId_);
	params.insert("item", item);
	message = QJsonRpcMessage::createRequest("Player.Open", params);
	client_->send(message);
}

void ImageControl::clearPlaylist()
{
	QJsonRpcMessage message;
	QJsonObject params;
	params.insert("playlistid", imagePlaylistId_);
	message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
	client_->send(message);
}

}
}
}

