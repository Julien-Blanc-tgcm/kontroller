#include "seasoninformationservice.h"
#include "client.h"
#include "tvshowepisodesrequest.h"
#include "utils.h"
#include <QStringList>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
int SeasonInformationService::seasonId() const
{
	return seasonId_;
}

void SeasonInformationService::setSeasonId(int seasonId)
{
	if (seasonId == seasonId_)
		return;
	seasonId_ = seasonId;
	emit seasonIdChanged();
}

QString SeasonInformationService::showTitle() const
{
	return showTitle_;
}

void SeasonInformationService::setShowTitle(const QString& showTitle)
{
	if (showTitle == showTitle_)
		return;
	showTitle_ = showTitle;
	emit showTitleChanged();
}

int SeasonInformationService::nbEpisodes() const
{
	return nbEpisodes_;
}

void SeasonInformationService::setNbEpisodes(int nbEpisodes)
{
	if (nbEpisodes == nbEpisodes_)
		return;
	nbEpisodes_ = nbEpisodes;
	emit nbEpisodesChanged();
}

int SeasonInformationService::nbWatchedEpisodes() const
{
	return nbWatchedEpisodes_;
}

void SeasonInformationService::setNbWatchedEpisodes(int nbWatchedEpisodes)
{
	if (nbWatchedEpisodes == nbWatchedEpisodes_)
		return;
	nbWatchedEpisodes_ = nbWatchedEpisodes;
	emit nbWatchedEpisodesChanged();
}

QString SeasonInformationService::fanart() const
{
	return fanart_;
}

void SeasonInformationService::setFanart(const QString& fanart)
{
	if (fanart_ == fanart)
		return;
	fanart_ = fanart;
	emit fanartChanged();
}

QString SeasonInformationService::art() const
{
	return art_;
}

void SeasonInformationService::setArt(const QString& art)
{
	if (art_ == art)
		return;
	art_ = art;
	emit artChanged();
}

QVector<File> SeasonInformationService::episodes() const
{
	return episodes_;
}

QVariantList SeasonInformationService::episodesList() const
{
	QVariantList ret;
	for (auto const& episode : episodes_)
	{
		ret.push_back(QVariant::fromValue(episode));
	}
	return ret;
}

QString SeasonInformationService::thumbnail() const
{
	return thumbnail_;
}

void SeasonInformationService::setThumbnail(const QString& thumbnail)
{
	if (thumbnail_ == thumbnail)
		return;
	thumbnail_ = thumbnail;
	emit thumbnailChanged();
}

SeasonInformationService::SeasonInformationService(QObject* parent) : QObject(parent)
{
}

void SeasonInformationService::refresh()
{
	QJsonObject parameters;
	parameters["tvshowid"] = tvShowId_;
	QJsonArray properties;
	properties.append(QString("season"));
	properties.append(QString("showtitle"));
	properties.append(QString("thumbnail"));
	properties.append(QString("episode"));
	properties.append(QString("watchedepisodes"));
	properties.append(QString("fanart"));
	properties.append(QString("art"));
	parameters["properties"] = properties;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("VideoLibrary.GetSeasons", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &SeasonInformationService::handleRefresh_);
	refreshEpisodes_();
}

void SeasonInformationService::setClient(Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	emit clientChanged(client_);
}

void SeasonInformationService::setTvShowId(int tvshowId)
{
	if (tvShowId_ == tvshowId)
		return;

	tvShowId_ = tvshowId;
	emit tvShowIdChanged(tvShowId_);
}

void SeasonInformationService::refreshEpisodes_()
{
	auto req = new TvShowEpisodesRequest(client_);
	connect(req, &TvShowEpisodesRequest::finished, this, &SeasonInformationService::handleRefreshEpisodes_);
	req->start(tvShowId_, seasonId_);
}

void SeasonInformationService::handleRefresh_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply)
	{
		auto result = reply->response().result();
		if (!result.isObject())
			return;
		auto seasonsTmp = result.toObject().value("seasons");
		if (!seasonsTmp.isArray())
			return;
		auto seasons = seasonsTmp.toArray();
		for (auto seasonTmp : seasons)
		{
			if (seasonTmp.isObject())
			{
				auto season = seasonTmp.toObject();
				if (static_cast<int>(season.value("season").toDouble()) == seasonId())
				{
					setShowTitle(season.value("showtitle").toString());
					setThumbnail(getImageUrl(client_, season.value("thumbnail").toString()).toString());
					setNbEpisodes(season.value("episode").toInt());
					setNbWatchedEpisodes(season.value("watchedepisodes").toInt());
					setFanart(getImageUrl(client_, season.value("fanart").toString()).toString());
					setArt(getImageUrl(client_, season.value("art").toString()).toString());
					// lastplayed
					// tags
				}
			}
		}
	}
}

void SeasonInformationService::handleRefreshEpisodes_()
{
	auto req = dynamic_cast<TvShowEpisodesRequest*>(sender());
	if (req)
	{
		episodes_ = req->episodes;
		req->episodes.clear();
		req->deleteLater();
		emit episodesChanged();
	}
}

Client* SeasonInformationService::client() const
{
	return client_;
}

int SeasonInformationService::tvShowId() const
{
	return tvShowId_;
}

} // namespace kontroller
} // namespace tgcm
} // namespace eu
