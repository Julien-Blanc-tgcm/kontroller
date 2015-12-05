#include "musicservice.h"
#include "kodiclient.h"
#include "kodisettingsmanager.h"

namespace {


int filesPropCount(QQmlListProperty<KodiFile>* list)
{
    return static_cast<QList<KodiFile*>*>(list->data)->count();
}

KodiFile* filesPropAt(QQmlListProperty<KodiFile>* list, int index)
{
    return static_cast<QList<KodiFile*>*>(list->data)->at(index);
}

}

bool MusicService::refreshing() const
{
    return refreshing_;
}

void MusicService::setRefreshing(bool refreshing)
{
    refreshing_ = refreshing;
    emit refreshingChanged();
}

MusicService::MusicService(QObject *parent) :
    QObject(parent),
    browsingMode_(""),
    browsingValue_(""),
    refreshing_(false)
{

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

QList<KodiFile *> MusicService::files() const
{
    return files_;
}

QQmlListProperty<KodiFile> MusicService::filesAsList()
{
    return QQmlListProperty<KodiFile>(this, &files_, &filesPropCount, &filesPropAt);
}

void MusicService::refresh()
{
    clearFiles();
    if(KodiSettingsManager::instance().musicFileBrowsing())
        refresh_files();
    else
    {
        if(browsingMode_ == "directory")
            refresh_files();
        else
            refresh_collection();
    }
}

void MusicService::setFiles(const QList<KodiFile *> &value)
{
    files_ = value;
}

void MusicService::playFile(QObject *file_)
{
    KodiFile* file = dynamic_cast<KodiFile*>(file_);
    if(file)
    {
        if(clearPlayList())
        {
            if(addFileToPlaylist(file))
                startPlaying();
        }
    }
    else
    {
        QString name = file_->objectName();
        file_->dumpObjectInfo();
        file_->dumpObjectTree();
        qDebug() << name;
    }
}

bool MusicService::clearPlayList()
{
    QJsonRpcMessage message;
    QJsonObject params;
    params.insert("playlistid", 0);
    message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
    KodiClient::current().send(message);
    //return reply.type() == QJsonRpcMessage::Response;
    return true;
}

bool MusicService::addFileToPlaylist(KodiFile* file)
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
    params.insert("playlistid", 0);
    message = QJsonRpcMessage::createRequest("Playlist.Add", params);
    KodiClient::current().send(message);
    //return reply.type() == QJsonRpcMessage::Response;
    return true;
}

bool MusicService::startPlaying()
{
    QJsonRpcMessage message;
    QJsonObject params;
    QJsonObject item;
    item.insert("playlistid", 0);
    params.insert("item", item);
    message = QJsonRpcMessage::createRequest("Player.Open", params);
    KodiClient::current().send(message);
    //return reply.type() == QJsonRpcMessage::Response;
    return true;
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
        QJsonObject obj;
        obj["order"] = QLatin1String("ascending");
        obj["method"] = QLatin1String("label");
        obj["ignorearticle"] = true;
        parameters.insert("sort", obj);
        message = QJsonRpcMessage::createRequest("Files.GetDirectory", parameters);
    }
    else
    {
        QJsonObject parameters;
        parameters.insert("media", QString::fromLatin1("music"));
        message = QJsonRpcMessage::createRequest("Files.GetSources", parameters);
    }
    QJsonRpcServiceReply* reply = KodiClient::current().send(message);
    if(reply)
        connect(reply, SIGNAL(finished()), this, SLOT(parseDirectoryResults()));
}

void MusicService::refresh_collection()
{
    setRefreshing(true);
    QJsonRpcMessage message;
    if(browsingMode_.size() > 0)
    {
        QJsonObject obj;
        obj["order"] = QLatin1String("ascending");
        obj["method"] = QLatin1String("label");
        obj["ignorearticle"] = true;
        QJsonObject parameters;
        parameters.insert("sort", obj);
        if(browsingMode_ == "media")
        {
            if(browsingValue_ == "artists")
            {
                message = QJsonRpcMessage::createRequest("AudioLibrary.GetArtists", parameters);
                QJsonRpcServiceReply* reply = KodiClient::current().send(message);
                if(reply)
                    connect(reply, SIGNAL(finished()), this, SLOT(parseArtistsResults()));
            }
            else if(browsingMode_ == "media" && browsingValue_ == "albums")
            {
                message = QJsonRpcMessage::createRequest("AudioLibrary.GetAlbums", parameters);
                QJsonRpcServiceReply* reply = KodiClient::current().send(message);
                if(reply)
                    connect(reply, SIGNAL(finished()), this, SLOT(parseAlbumsResults()));
            }
            else if(browsingValue_ == "songs")
            {
                message = QJsonRpcMessage::createRequest("AudioLibrary.GetSongs", parameters);
                QJsonRpcServiceReply* reply = KodiClient::current().send(message);
                if(reply)
                    connect(reply, SIGNAL(finished()), this, SLOT(parseSongsResults()));
            }
            else if(browsingValue_ == "genres")
            {
                message = QJsonRpcMessage::createRequest("AudioLibrary.GetGenres", parameters);
                QJsonRpcServiceReply* reply = KodiClient::current().send(message);
                if(reply)
                    connect(reply, SIGNAL(finished()), this, SLOT(parseGenresResults()));
            }
        }
        else if(browsingMode_ == "artist")
        {
            QJsonObject filter;
            filter["artistid"] = browsingValue_.toInt();
            parameters.insert("filter", filter);
            message = QJsonRpcMessage::createRequest("AudioLibrary.GetAlbums", parameters);
            QJsonRpcServiceReply* reply = KodiClient::current().send(message);
            if(reply)
                connect(reply, SIGNAL(finished()), this, SLOT(parseAlbumsResults()));
        }
        else if(browsingMode_ == "album")
        {
            QJsonObject filter;
            filter["albumid"] = browsingValue_.toInt();
            parameters.insert("filter", filter);
            message = QJsonRpcMessage::createRequest("AudioLibrary.GetSongs", parameters);
            QJsonRpcServiceReply* reply = KodiClient::current().send(message);
            if(reply)
                connect(reply, SIGNAL(finished()), this, SLOT(parseSongsResults()));
        }
        else if(browsingMode_ == "genre")
        {
            QJsonObject filter;
            filter["genreid"] = browsingValue_.toInt();
            parameters.insert("filter", filter);
            message = QJsonRpcMessage::createRequest("AudioLibrary.GetAlbums", parameters);
            QJsonRpcServiceReply* reply = KodiClient::current().send(message);
            if(reply)
                connect(reply, SIGNAL(finished()), this, SLOT(parseAlbumsResults()));
        }
    }
    else
    {
        KodiFile* file = new KodiFile(this);
        file->setLabel(tr("Artists"));
        file->setFile("artists");
        file->setType("media");
        file->setFiletype("media");
        files_.push_back(file);
        file = new KodiFile(this);
        file->setFile("albums");
        file->setLabel(tr("Albums"));
        file->setType("media");
        file->setFiletype("media");
        files_.push_back(file);
        file = new KodiFile(this);
        file->setLabel(tr("Songs"));
        file->setFile("songs");
        file->setType("media");
        file->setFiletype("media");
        files_.push_back(file);
        file = new KodiFile(this);
        file->setLabel(tr("Genres"));
        file->setFile("genres");
        file->setType("media");
        file->setFiletype("media");
        files_.push_back(file);
        file = new KodiFile(this);
        file->setLabel(tr("Files"));
        file->setFile("");
        file->setType("directory");
        file->setFiletype("directory");
        files_.push_back(file);
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
                        KodiFile* file = new KodiFile(this);
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
                files = result.toObject().take("albums");
                if(files.type() == QJsonValue::Array)
                {
                    QJsonArray res = files.toArray();
                    for(QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
                    {
                        KodiFile* file = new KodiFile(this);
                        if((*it).type() == QJsonValue::Object)
                        {
                            QJsonObject obj = (*it).toObject();
                            QJsonValue val = obj.value("label");
                            if(val.type() == QJsonValue::String)
                                file->setLabel(val.toString());
                            val = obj.value("albumid");
                            if(val.type() == QJsonValue::Double)
                                file->setFile(QString::number(val.toDouble()));
                            file->setFiletype("album");
                            file->setType("album");
                            files_.push_back(file);
                        }
                    }
                }
            }
        }
    }
    emit filesAsListChanged();
    setRefreshing(false);
    sender()->deleteLater();
}

void MusicService::parseSongsResults()
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
                files = result.toObject().take("songs");
                if(files.type() == QJsonValue::Array)
                {
                    QJsonArray res = files.toArray();
                    for(QJsonArray::const_iterator it = res.begin(); it != res.end(); ++it)
                    {
                        KodiFile* file = new KodiFile(this);
                        if((*it).type() == QJsonValue::Object)
                        {
                            QJsonObject obj = (*it).toObject();
                            QJsonValue val = obj.value("label");
                            if(val.type() == QJsonValue::String)
                                file->setLabel(val.toString());
                            val = obj.value("songid");
                            if(val.type() == QJsonValue::Double)
                                file->setFile(QString::number(val.toDouble()));
                            file->setFiletype("song");
                            file->setType("song");
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
                        KodiFile* file = new KodiFile(this);
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
                        KodiFile* file = new KodiFile(this);
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
