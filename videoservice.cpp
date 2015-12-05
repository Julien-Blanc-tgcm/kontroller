#include "videoservice.h"
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

bool VideoService::refreshing() const
{
    return refreshing_;
}

void VideoService::setRefreshing(bool refreshing)
{
    refreshing_ = refreshing;
    emit refreshingChanged();
}

VideoService::VideoService(QObject *parent) :
    QObject(parent),
    browsingMode_(""),
    browsingValue_(""),
    refreshing_(false)
{

}

VideoService::VideoService(QString browsingMode, QString browsingValue, QObject* parent) :
    QObject(parent),
    browsingMode_(browsingMode),
    browsingValue_(browsingValue),
    refreshing_(false)
{
}

VideoService::~VideoService()
{
    clearFiles();
}

void VideoService::clearFiles()
{
    /*for(int i = 0; i < files_.count(); ++i)
        delete files_[i];*/
    files_.clear();
}

QList<KodiFile *> VideoService::files() const
{
    return files_;
}

QQmlListProperty<KodiFile> VideoService::filesAsList()
{
    return QQmlListProperty<KodiFile>(this, &files_, &filesPropCount, &filesPropAt);
}

void VideoService::refresh()
{
    clearFiles();
    if(KodiSettingsManager::instance().videosFileBrowsing())
        refresh_files();
    else
    {
        if(browsingMode_ == "directory")
            refresh_files();
        else
            refresh_collection();
    }
}

void VideoService::setFiles(const QList<KodiFile *> &value)
{
    files_ = value;
}

void VideoService::playFile(QObject *file_)
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

bool VideoService::clearPlayList()
{
    QJsonRpcMessage message;
    QJsonObject params;
    params.insert("playlistid", 0);
    message = QJsonRpcMessage::createRequest("Playlist.Clear", params);
    KodiClient::current().send(message);
    //return reply.type() == QJsonRpcMessage::Response;
    return true;
}

bool VideoService::addFileToPlaylist(KodiFile* file)
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

bool VideoService::startPlaying()
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

void VideoService::setBrowsingMode(QString browsingMode)
{
    browsingMode_ = browsingMode;
    emit browsingModeChanged();
}

void VideoService::setBrowsingValue(QString browsingValue)
{
    browsingValue_ = browsingValue;
    emit browsingValueChanged();
}

void VideoService::setLabel(QString label)
{
    label_ = label;
}

QString VideoService::browsingMode() const
{
    return browsingMode_;
}

QString VideoService::browsingValue() const
{
    return browsingValue_;
}

QString VideoService::label() const
{
    return label_;
}

void VideoService::refresh_files()
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
        parameters.insert("media", QString::fromLatin1("video"));
        message = QJsonRpcMessage::createRequest("Files.GetSources", parameters);
    }
    QJsonRpcServiceReply* reply = KodiClient::current().send(message);
    if(reply)
        connect(reply, SIGNAL(finished()), this, SLOT(parseDirectoryResults()));
}

void VideoService::refresh_collection()
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
            if(browsingValue_ == "movies")
            {
                message = QJsonRpcMessage::createRequest("VideoLibrary.GetMovies", parameters);
                QJsonRpcServiceReply* reply = KodiClient::current().send(message);
                if(reply)
                    connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseMoviesResults_);
            }
            else if(browsingValue_ == "tvshows")
            {
                message = QJsonRpcMessage::createRequest("VideoLibrary.GetTVShows", parameters);
                QJsonRpcServiceReply* reply = KodiClient::current().send(message);
                if(reply)
                    connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseTVShowsResults_);
            }
            else if(browsingValue_ == "musicvideos")
            {
                message = QJsonRpcMessage::createRequest("VideoLibrary.GetMusicVideos", parameters);
                QJsonRpcServiceReply* reply = KodiClient::current().send(message);
                if(reply)
                    connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseMusicVideosResults_);
            }
            else if(browsingValue_ == "genres")
            {
                message = QJsonRpcMessage::createRequest("VideoLibrary.GetGenres", parameters);
                QJsonRpcServiceReply* reply = KodiClient::current().send(message);
                if(reply)
                    connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseGenresResults_);
            }
        }
        else if(browsingMode_ == "tvshow")
        {
            parameters.insert("tvshowid", browsingValue_.toInt());
            QJsonArray properties;
            properties.append(QString("season"));
            parameters.insert("properties", properties);
            message = QJsonRpcMessage::createRequest("VideoLibrary.GetSeasons", parameters);
            QJsonRpcServiceReply* reply = KodiClient::current().send(message);
            if(reply)
                connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseSeasonsResults_);
        }
        else if(browsingMode_ == "season")
        {
            QStringList list = browsingValue_.split('|');
            if(list.size() != 2)
            {
                setRefreshing(false);
                return;
            }
            parameters.insert("season", list[1].toInt());
            parameters.insert("tvshowid", list[0].toInt());
            message = QJsonRpcMessage::createRequest("VideoLibrary.GetEpisodes", parameters);
            QJsonRpcServiceReply* reply = KodiClient::current().send(message);
            if(reply)
                connect(reply, &QJsonRpcServiceReply::finished, this, &VideoService::parseEpisodesResults_);
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
        else
            setRefreshing(false);
    }
    else
    {
        KodiFile* file = new KodiFile(this);
        file->setLabel(tr("Movies"));
        file->setFile("movies");
        file->setType("media");
        file->setFiletype("media");
        files_.push_back(file);
        file = new KodiFile(this);
        file->setLabel(tr("TV Shows"));
        file->setFile("tvshows");
        file->setType("media");
        file->setFiletype("media");
        files_.push_back(file);
        file = new KodiFile(this);
        file->setLabel(tr("Clips"));
        file->setFile("musicvideos");
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

void VideoService::parseMoviesResults_()
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
                files = result.toObject().take("movies");
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
                            val = obj.value("movieid");
                            if(val.type() == QJsonValue::Double)
                                file->setFile(QString::number(val.toDouble()));
                            file->setFiletype("movie");
                            file->setType("movie");
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

void VideoService::parseTVShowsResults_()
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
                files = result.toObject().take("tvshows");
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
                            val = obj.value("tvshowid");
                            if(val.type() == QJsonValue::Double)
                                file->setFile(QString::number(val.toDouble()));
                            file->setFiletype("tvshow");
                            file->setType("tvshow");
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

void VideoService::parseMusicVideosResults_()
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
                files = result.toObject().take("musicvideos");
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
                            val = obj.value("musicvideoid");
                            if(val.type() == QJsonValue::Double)
                                file->setFile(QString::number(val.toDouble()));
                            file->setFiletype("musicvideo");
                            file->setType("musicvideo");
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

void VideoService::parseGenresResults_()
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

void VideoService::parseSeasonsResults_()
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
                files = result.toObject().take("seasons");
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
                            val = obj.value("season");
                            if(val.type() == QJsonValue::Double)
                                file->setFile(browsingValue_ + "|" + QString::number(val.toDouble()));
                            file->setFiletype("season");
                            file->setType("season");
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

void VideoService::parseEpisodesResults_()
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
                files = result.toObject().take("episodes");
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
                            val = obj.value("episodeid");
                            if(val.type() == QJsonValue::Double)
                                file->setFile(QString::number(val.toDouble()));
                            file->setFiletype("episode");
                            file->setType("episode");
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

void VideoService::parseDirectoryResults()
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
