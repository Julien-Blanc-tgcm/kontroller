#include "albuminformationservice.h"
#include "kodiclient.h"
#include "songsrequest.h"
#include "utils.h"

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

QString AlbumInformationService::name() const
{
    return name_;
}

void AlbumInformationService::setName(const QString &name)
{
    name_ = name;
    emit nameChanged();
}

QString AlbumInformationService::description() const
{
    return description_;
}

void AlbumInformationService::setDescription(const QString &description)
{
    description_ = description;
    emit descriptionChanged();
}

QString AlbumInformationService::thumbnail() const
{
    return thumbnail_;
}

void AlbumInformationService::setThumbnail(const QString &value)
{
    thumbnail_ = value;
    emit thumbnailChanged();
}

QQmlListProperty<KodiFile> AlbumInformationService::songs()
{
    return QQmlListProperty<KodiFile>(this, &songs_, &filesPropCount, &filesPropAt);
}

int AlbumInformationService::albumId() const
{
    return albumId_;
}

void AlbumInformationService::setAlbumId(int albumId)
{
    albumId_ = albumId;
    emit albumIdChanged();
}

QString AlbumInformationService::genre() const
{
    return genre_;
}

void AlbumInformationService::setGenre(const QString &genre)
{
    genre_ = genre;
    emit genreChanged();
}

int AlbumInformationService::year() const
{
    return year_;
}

void AlbumInformationService::setYear(const int &year)
{
    year_ = year;
    emit yearChanged();
}

QString AlbumInformationService::label() const
{
    return label_;
}

void AlbumInformationService::setLabel(const QString &label)
{
    label_ = label;
    emit labelChanged();
}

QStringList AlbumInformationService::artists() const
{
    return artists_;
}

void AlbumInformationService::setArtists(const QStringList &artists)
{
    artists_ = artists;
    emit artistsChanged();
}

AlbumInformationService::AlbumInformationService(QObject* parent) : QObject(parent),
    year_(0)
{

}

void AlbumInformationService::refresh()
{
    QJsonObject parameters;
    parameters["albumid"] = albumId_;
    QJsonArray properties;
    properties.append(QString("title"));
    properties.append(QString("description"));
    properties.append(QString("artist"));
    properties.append(QString("artistid"));
    properties.append(QString("genre"));
//    properties.append(QString("genreid"));
    properties.append(QString("fanart"));
    properties.append(QString("albumlabel"));
    properties.append(QString("year"));
    properties.append(QString("thumbnail"));
    properties.append(QString("rating"));
    parameters["properties"] = properties;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("AudioLibrary.GetAlbumDetails", parameters);
    auto reply = KodiClient::current().send(message);
    connect(reply, &QJsonRpcServiceReply::finished, this, &AlbumInformationService::handleRefresh_);
    auto songsQuery = new SongsRequest();
    connect(songsQuery, &SongsRequest::finished, this, &AlbumInformationService::handleSongs_);
    songsQuery->start(albumId_);
}

void AlbumInformationService::handleRefresh_()
{
    auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        auto result = reply->response().result();
        if(!result.isObject())
            return;
        auto detailsTmp = result.toObject().value("albumdetails");
        if(!detailsTmp.isObject())
            return;
        auto details = detailsTmp.toObject();
        setName(details.value("title").toString());
        setDescription(details.value("description").toString());
        setGenre(details.value("genre").toString());
        setYear(details.value("year").toInt());
        auto artists = details.value("artist");
        if(artists.isArray())
        {
            auto artistsArray = artists.toArray();
            for(auto const& val : artistsArray)
            {
                if(val.isString())
                    artists_.push_back(val.toString());
            }
        }
        emit artistsChanged();
        setLabel(details.value("albumlabel").toString());
        setThumbnail(getImageUrl(details.value("thumbnail").toString()).toString());
    }
}

void AlbumInformationService::handleSongs_()
{
    auto songsQuery = dynamic_cast<SongsRequest*>(sender());
    if(songsQuery)
    {
        songs_ = std::move(songsQuery->results);
        emit songsChanged();
        songsQuery->deleteLater();
    }
}
