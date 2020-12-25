#include "tvshowinformationservice.h"

#include "client.h"
#include "file.h"
#include "requests/tvshowseasonsrequest.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

QString TvShowInformationService::title() const
{
	return title_;
}

void TvShowInformationService::setTitle(const QString& title)
{
	title_ = title;
	emit titleChanged();
}

int TvShowInformationService::year() const
{
	return year_;
}

void TvShowInformationService::setYear(int year)
{
	year_ = year;
	emit yearChanged();
}

QString TvShowInformationService::plot() const
{
	return plot_;
}

void TvShowInformationService::setPlot(const QString& plot)
{
	plot_ = plot;
	emit plotChanged();
}

double TvShowInformationService::rating() const
{
	return rating_;
}

void TvShowInformationService::setRating(double rating)
{
	rating_ = rating;
	emit ratingChanged();
}

int TvShowInformationService::nbSeasons() const
{
	return nbSeasons_;
}

void TvShowInformationService::setNbSeasons(int nbSeasons)
{
	nbSeasons_ = nbSeasons;
	emit nbSeasonsChanged();
}

QString TvShowInformationService::studio() const
{
	return studio_;
}

void TvShowInformationService::setStudio(const QString& studio)
{
	studio_ = studio;
	emit studioChanged();
}

int TvShowInformationService::nbEpisodes() const
{
	return nbEpisodes_;
}

void TvShowInformationService::setNbEpisodes(int nbEpisodes)
{
	nbEpisodes_ = nbEpisodes;
	emit nbEpisodesChanged();
}

int TvShowInformationService::nbWatchedEpisodes() const
{
	return nbWatchedEpisodes_;
}

void TvShowInformationService::setNbWatchedEpisodes(int nbWatchedEpisodes)
{
	nbWatchedEpisodes_ = nbWatchedEpisodes;
	emit nbWatchedEpisodesChanged();
}

QString TvShowInformationService::fanart() const
{
	return fanart_;
}

void TvShowInformationService::setFanart(const QString& fanart)
{
	if (fanart != fanart_)
	{
		fanart_ = fanart;
		emit fanartChanged();
	}
}

QString TvShowInformationService::file() const
{
	return file_;
}

void TvShowInformationService::setFile(const QString& file)
{
	file_ = file;
	emit fileChanged();
}

QString TvShowInformationService::originalTitle() const
{
	return originalTitle_;
}

void TvShowInformationService::setOriginalTitle(const QString& originalTitle)
{
	originalTitle_ = originalTitle;
	emit originalTitleChanged();
}

QString TvShowInformationService::dateAdded() const
{
	return dateAdded_;
}

void TvShowInformationService::setDateAdded(const QString& dateAdded)
{
	dateAdded_ = dateAdded;
	emit dateAddedChanged();
}

QString TvShowInformationService::art() const
{
	return art_;
}

void TvShowInformationService::setArt(const QString& art)
{
	art_ = art;
	emit artChanged();
}

QString TvShowInformationService::thumbnail() const
{
	return thumbnail_;
}

void TvShowInformationService::setThumbnail(const QString& thumbnail)
{
	thumbnail_ = thumbnail;
	emit thumbnailChanged();
}

TvShowInformationService::TvShowInformationService(QObject* parent) : QObject(parent)
{
}

int TvShowInformationService::tvshowId() const
{
	return tvshowId_;
}

void TvShowInformationService::setTvshowId(int tvshowId)
{
	tvshowId_ = tvshowId;
	emit tvshowIdChanged();
}

void TvShowInformationService::refresh()
{
	QJsonObject parameters;
	parameters["tvshowid"] = tvshowId_;
	QJsonArray properties;
	properties.append(QString("title"));
	properties.append(QString("thumbnail"));
	properties.append(QString("year"));
	properties.append(QString("genre"));
	properties.append(QString("plot"));
	properties.append(QString("rating"));
	properties.append(QString("season"));
	properties.append(QString("studio"));
	properties.append(QString("cast"));
	properties.append(QString("episode"));
	properties.append(QString("watchedepisodes"));
	properties.append(QString("fanart"));
	properties.append(QString("lastplayed"));
	properties.append(QString("file"));
	properties.append(QString("originaltitle"));
	properties.append(QString("episodeguide"));
	properties.append(QString("dateadded"));
	properties.append(QString("tag"));
	properties.append(QString("art"));
	parameters["properties"] = properties;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("VideoLibrary.GetTvShowDetails", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &TvShowInformationService::handleRefresh_);
	refreshSeasons_();
}

void TvShowInformationService::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	emit clientChanged(client_);
}

void TvShowInformationService::handleRefresh_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		auto result = reply->response().result();
		if (!result.isObject())
			return;
		auto detailsTmp = result.toObject().value("tvshowdetails");
		if (!detailsTmp.isObject())
			return;
		auto details = detailsTmp.toObject();
		setTitle(details.value("title").toString());
		setThumbnail(getImageUrl(client_, details.value("thumbnail").toString()).toString());
		setYear(details.value("year").toInt());
		// genre
		auto genres = details.value("genre").toArray();
		if (!genres.isEmpty())
		{
			for (auto genre : genres)
			{
				if (genre.isString())
					genres_.push_back(genre.toString());
			}
		}
		emit genresChanged();
		setPlot(details.value("plot").toString().replace("\r", ""));
		setRating(details.value("rating").toDouble());
		setNbSeasons(details.value("season").toInt());
		setStudio(details.value("studio").toString());
		// cast
		setNbEpisodes(details.value("episode").toInt());
		setNbWatchedEpisodes(details.value("watchedepisodes").toInt());
		setFanart(getImageUrl(client_, details.value("fanart").toString()).toString());
		// lastplayed
		setFile(details.value("file").toString());
		setOriginalTitle(details.value("originalTitle").toString());
		// episodeguide
		setDateAdded(details.value("dateadded").toString());
		// tags
		/*        QJsonValue val = details.value("art");
		        if(val.isObject())
		            setArt(getImageUrl(val.toObject().value("banner").toString()).toString()); */
	}
}

QVariantList TvShowInformationService::seasons()
{
	QVariantList ret;
	ret.reserve(seasons_.size());
	for (auto const& s : seasons_)
	{
		ret.push_back(QVariant::fromValue(s));
	}
	return ret;
}

QStringList TvShowInformationService::genres()
{
	return genres_;
}

Client* TvShowInformationService::client() const
{
	return client_;
}

void TvShowInformationService::refreshSeasons_()
{
	auto seasonsQuery = new TvShowSeasonsRequest(client_, this);
	connect(seasonsQuery, &TvShowSeasonsRequest::finished, this, &TvShowInformationService::handleSeasons_);
	seasonsQuery->start(this->tvshowId());
}

void TvShowInformationService::handleSeasons_()
{
	auto seasonsQuery = dynamic_cast<TvShowSeasonsRequest*>(sender());
	if (seasonsQuery)
	{
		seasons_ = seasonsQuery->seasons;
		emit seasonsChanged();
	}
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
