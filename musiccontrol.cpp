#include "musiccontrol.h"
#include "client.h"
#include "kodifile.h"
MusicControl::MusicControl(QObject *parent) : QObject(parent)
{

}

void MusicControl::playFile(KodiFile *file)
{
    if(file)
    {
        clearPlaylist();
        addToPlaylist(file);
        startPlaying();
    }
}

void MusicControl::addToPlaylist(KodiFile *file)
{
    if(file)
    {
        QJsonRpcMessage message;
        QJsonObject params;
        QJsonObject item;
        if(file->filetype() == "directory")
            item.insert("directory", file->file());
        else if(file->filetype() == "file")
            item.insert("file", file->file());
        else if(file->filetype() == "album")
            item.insert("albumid", file->file().toInt());
        else if(file->filetype() == "song")
            item.insert("songid", file->file().toInt());
        params.insert("item", item);
        params.insert("playlistid", audioPlaylistId_);
        message = QJsonRpcMessage::createRequest("Playlist.Add", params);
        Client::current().send(message);
    }
}

void MusicControl::startPlaying()
{
    QJsonRpcMessage message;
    QJsonObject params;
    QJsonObject item;
    item.insert("playlistid", audioPlaylistId_);
    params.insert("item", item);
    message = QJsonRpcMessage::createRequest("Player.Open", params);
    Client::current().send(message);
}

void MusicControl::clearPlaylist()
{
    QJsonRpcMessage message;
    QJsonObject params;
    params.insert("playlistid", audioPlaylistId_);
    message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
    Client::current().send(message);
}
