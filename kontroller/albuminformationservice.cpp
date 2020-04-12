#include "albuminformationservice.h"
#include "client.h"
#include "songsrequest.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

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

QVariantList AlbumInformationService::songs()
{
	QVariantList l;
	for(auto f : songs_)
		l.push_back(QVariant::fromValue(f));
	return l;
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

QStringList AlbumInformationService::genres() const
{
    return genres_;
}

void AlbumInformationService::setGenres(const QStringList &genre)
{
    genres_ = genre;
    emit genresChanged();
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

Client* AlbumInformationService::client() const
{
	return client_;
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
	auto reply = client_->send(message);
    connect(reply, &QJsonRpcServiceReply::finished, this, &AlbumInformationService::handleRefresh_);
	auto songsQuery = new SongsRequest(client_);
    connect(songsQuery, &SongsRequest::finished, this, &AlbumInformationService::handleSongs_);
	songsQuery->start(albumId_);
}

void AlbumInformationService::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	emit clientChanged(client_);
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
        auto genres = details.value("genre");
        QStringList genresTmp;
        if(genres.isArray())
        {
            auto arr = genres.toArray();
            for(auto const& genre : arr)
            {
                if(genre.isString())
                    genresTmp.push_back(genre.toString());
            }

        }
        setGenres(genresTmp);
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
		setThumbnail(getImageUrl(client_, details.value("thumbnail").toString()).toString());
    }
}

void AlbumInformationService::handleSongs_()
{
    auto songsQuery = dynamic_cast<SongsRequest*>(sender());
    if(songsQuery)
    {
		songs_ = songsQuery->results;
        emit songsChanged();
        songsQuery->deleteLater();
    }
}

}
}
}
