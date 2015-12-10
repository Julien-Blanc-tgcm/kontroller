#include "videocontrol.h"

#include "kodiclient.h"
#include "kodifile.h"

VideoControl::VideoControl(QObject *parent) : QObject(parent)
{

}

void VideoControl::playFile(KodiFile *file)
{
    if(file)
    {
        clearPlaylist();
        addFileToPlaylist(file);
        startPlaying();
    }
}

void VideoControl::clearPlaylist()
{
    QJsonRpcMessage message;
    QJsonObject params;
    params.insert("playlistid", videoPlaylistId_);
    message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
    KodiClient::current().send(message);
}

void VideoControl::addFileToPlaylist(KodiFile* file)
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
        params.insert("item", item);
        params.insert("playlistid", videoPlaylistId_);
        message = QJsonRpcMessage::createRequest("Playlist.Add", params);
        KodiClient::current().send(message);
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
    KodiClient::current().send(message);
}
