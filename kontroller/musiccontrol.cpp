#include "musiccontrol.h"
#include "client.h"
#include "file.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

MusicControl::MusicControl(QObject* parent) : QObject(parent)
{
}

Client* MusicControl::client() const
{
	return client_;
}

void MusicControl::playFile(File file)
{
	QJsonObject params;
	QJsonObject item;
	if (file.filetype() == "directory" || file.filetype() == "playlist")
		item.insert("directory", file.file());
	else if (file.filetype() == "file")
		item.insert("file", file.file());
	else if (file.filetype() == "album")
		item.insert("albumid", file.id());
	else if (file.filetype() == "song")
		item.insert("songid", file.id());
	params.insert("item", item);
	params.insert("options", QJsonObject{});
	auto message = QJsonRpcMessage::createRequest("Player.Open", params);
	client_->send(message);
}

void MusicControl::addToPlaylist(File file)
{
	QJsonRpcMessage message;
	QJsonObject params;
	QJsonObject item;
	if (file.filetype() == "directory" || file.filetype() == "playlist")
		item.insert("directory", file.file());
	else if (file.filetype() == "file")
		item.insert("file", file.file());
	else if (file.filetype() == "album")
		item.insert("albumid", file.id());
	else if (file.filetype() == "song")
		item.insert("songid", file.id());
	params.insert("item", item);
	params.insert("playlistid", audioPlaylistId_);
	message = QJsonRpcMessage::createRequest("Playlist.Add", params);
	client_->send(message);
}

void MusicControl::startPlaying()
{
	QJsonRpcMessage message;
	QJsonObject params;
	QJsonObject item;
	item.insert("playlistid", audioPlaylistId_);
	params.insert("item", item);
	message = QJsonRpcMessage::createRequest("Player.Open", params);
	client_->send(message);
}

void MusicControl::clearPlaylist()
{
	QJsonRpcMessage message;
	QJsonObject params;
	params.insert("playlistid", audioPlaylistId_);
	message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
	client_->send(message);
}

void MusicControl::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	emit clientChanged(client_);
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
