#include "artistinformationservice.h"

#include "albumsrequest.h"
#include "client.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

int ArtistInformationService::artistId() const
{
	return artistId_;
}

void ArtistInformationService::setArtistId(int artistId)
{
	artistId_ = artistId;
	emit artistIdChanged();
}

QString ArtistInformationService::artistName() const
{
	return artistName_;
}

void ArtistInformationService::setArtistName(const QString& artistName)
{
	artistName_ = artistName;
	emit artistNameChanged();
}

QString ArtistInformationService::artistDescription() const
{
	return artistDescription_;
}

void ArtistInformationService::setArtistDescription(const QString& artistDescription)
{
	artistDescription_ = artistDescription;
	emit artistDescriptionChanged();
}

QString ArtistInformationService::fanart() const
{
	return fanart_;
}

void ArtistInformationService::setFanart(const QString& fanart)
{
	fanart_ = fanart;
	emit fanartChanged();
}

QString ArtistInformationService::thumbnail() const
{
	return thumbnail_;
}

void ArtistInformationService::setThumbnail(const QString& thumbnail)
{
	thumbnail_ = thumbnail;
	emit thumbnailChanged();
}

QString ArtistInformationService::genres() const
{
	if (genres_.empty())
		return QString("");
	QString ret = genres_[0];
	for (int i = 1; i < genres_.size(); ++i)
	{
		ret = "," + genres_[i];
	}
	return ret;
}

QString ArtistInformationService::style() const
{
	if (style_.empty())
		return QString("");
	QString ret = style_[0];
	for (int i = 1; i < style_.size(); ++i)
	{
		ret = "," + style_[i];
	}
	return ret;
}

QVariantList ArtistInformationService::albums()
{
	QVariantList l;
	for (auto const& f : albums_)
		l.push_back(QVariant::fromValue(f));
	return l;
}

Client* ArtistInformationService::client() const
{
	return client_;
}

ArtistInformationService::ArtistInformationService(QObject* parent) : QObject(parent)
{
}

void ArtistInformationService::refresh()
{
	QJsonObject parameters;
	parameters["artistid"] = artistId_;
	QJsonArray properties;
	properties.append(QString("style"));
	properties.append(QString("description"));
	properties.append(QString("genre"));
	properties.append(QString("fanart"));
	properties.append(QString("thumbnail"));
	parameters["properties"] = properties;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("AudioLibrary.GetArtistDetails", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &ArtistInformationService::handleRefresh_);
	auto albumsQuery = new AlbumsRequest(client_, this);
	connect(albumsQuery, &AlbumsRequest::finished, this, &ArtistInformationService::handleAlbums_);
	albumsQuery->start(artistId_);
}

void ArtistInformationService::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	emit clientChanged(client_);
}

void ArtistInformationService::handleRefresh_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		auto result = reply->response().result();
		if (!result.isObject())
			return;
		auto detailsTmp = result.toObject().value("artistdetails");
		if (!detailsTmp.isObject())
			return;
		auto details = detailsTmp.toObject();
		setArtistName(details.value("label").toString());
		setArtistDescription(details.value("description").toString());
		auto const genreTmp = details.value("genre");
		if (genreTmp.isArray())
		{
			auto const genre = genreTmp.toArray();
			genres_.clear();
			for (auto const& value : genre)
			{
				if (value.isString())
					genres_.push_back(value.toString());
			}
			emit genresChanged();
		}
		auto const styleTmp = details.value("style");
		if (styleTmp.isArray())
		{
			auto const style = styleTmp.toArray();
			style_.clear();
			for (auto const& value : style)
			{
				if (value.isString())
					style_.push_back(value.toString());
			}
			emit styleChanged();
		}
		setFanart(getImageUrl(client_, details.value("fanart").toString()).toString());
		setThumbnail(getImageUrl(client_, details.value("thumbnail").toString()).toString());
	}
}

void ArtistInformationService::handleAlbums_()
{
	auto albumsQuery = dynamic_cast<AlbumsRequest*>(sender());
	if (albumsQuery)
	{
		albums_ = albumsQuery->results;
		emit albumsChanged();
	}
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
