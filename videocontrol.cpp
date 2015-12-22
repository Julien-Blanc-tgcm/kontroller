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

void VideoControl::playFile(File *file)
{
    if(file)
    {
        clearPlaylist();
        addToPlaylist(file);
        startPlaying();
    }
}

void VideoControl::clearPlaylist()
{
    QJsonRpcMessage message;
    QJsonObject params;
    params.insert("playlistid", videoPlaylistId_);
    message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
    Client::current().send(message);
}

void VideoControl::addToPlaylist(File* file)
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
        else if(file->filetype() == "movie")
            item.insert("movieid", file->file().toInt());
        else if(file->filetype() == "tvshow")
            item.insert("tvshowid", file->file().toInt());
        else if(file->filetype() == "episode")
            item.insert("episodeid", file->file().toInt());
        else if(file->filetype() == "musicvideo")
            item.insert("musicvideoid", file->file().toInt());
        params.insert("item", item);
        params.insert("playlistid", videoPlaylistId_);
        message = QJsonRpcMessage::createRequest("Playlist.Add", params);
        Client::current().send(message);
    }
}

void VideoControl::startPlaying()
{
    QJsonRpcMessage message;
    QJsonObject params;
    QJsonObject item;
    item.insert("playlistid", videoPlaylistId_);
    params.insert("item", item);
    message = QJsonRpcMessage::createRequest("Player.Open", params);
    Client::current().send(message);
}

}
}
}
