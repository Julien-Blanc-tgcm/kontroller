#include "seasoninformationservice.h"
#include "client.h"
#include <QStringList>
#include "tvshowepisodesrequest.h"
#include "utils.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{
QString SeasonInformationService::seasonId() const
{
    return seasonId_;
}

void SeasonInformationService::setSeasonId(const QString &seasonId)
{
    seasonId_ = seasonId;
    emit seasonIdChanged();
}

QString SeasonInformationService::showTitle() const
{
    return showTitle_;
}

void SeasonInformationService::setShowTitle(const QString &showTitle)
{
    showTitle_ = showTitle;
    emit showTitleChanged();
}

int SeasonInformationService::nbEpisodes() const
{
    return nbEpisodes_;
}

void SeasonInformationService::setNbEpisodes(int nbEpisodes)
{
    nbEpisodes_ = nbEpisodes;
    emit nbEpisodesChanged();
}

int SeasonInformationService::nbWatchedEpisodes() const
{
    return nbWatchedEpisodes_;
}

void SeasonInformationService::setNbWatchedEpisodes(int nbWatchedEpisodes)
{
    nbWatchedEpisodes_ = nbWatchedEpisodes;
    emit nbWatchedEpisodesChanged();
}

QString SeasonInformationService::fanart() const
{
    return fanart_;
}

void SeasonInformationService::setFanart(const QString &fanart)
{
    fanart_ = fanart;
    emit fanartChanged();
}

QString SeasonInformationService::art() const
{
    return art_;
}

void SeasonInformationService::setArt(const QString &art)
{
    art_ = art;
    emit artChanged();
}

QVector<File> SeasonInformationService::episodes()
{
	return episodes_;
}

QString SeasonInformationService::thumbnail() const
{
    return thumbnail_;
}

void SeasonInformationService::setThumbnail(const QString& thumbnail)
{
    thumbnail_ = thumbnail;
    emit thumbnailChanged();
}

SeasonInformationService::SeasonInformationService(QObject *parent) : QObject(parent)
{

}

void SeasonInformationService::refresh()
{
    QJsonObject parameters;
    QStringList list = seasonId_.split(QChar('|'));
    if(list.size() != 2)
        return;
    parameters["tvshowid"] = list[0].toInt();
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

void SeasonInformationService::refreshEpisodes_()
{
	QStringList list = seasonId_.split(QChar('|'));
	if(list.size() != 2)
		return;
	auto req = new TvShowEpisodesRequest(client_);
    connect(req, &TvShowEpisodesRequest::finished, this, &SeasonInformationService::handleRefreshEpisodes_);
	req->start(list[0].toInt(), list[1].toInt());
}

void SeasonInformationService::handleRefresh_()
{
    QStringList list = seasonId_.split(QChar('|'));
    if(list.size() != 2)
        return;
    auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        auto result = reply->response().result();
        if(!result.isObject())
            return;
        auto seasonsTmp = result.toObject().value("seasons");
        if(!seasonsTmp.isArray())
            return;
        auto seasons = seasonsTmp.toArray();
        for(auto seasonTmp : seasons)
        {
            if(seasonTmp.isObject())
            {
                auto season = seasonTmp.toObject();
                if(season.value("season").toDouble() == list[1].toDouble())
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
	if(req)
	{
		episodes_ = req->episodes;
		req->episodes.clear();
		req->deleteLater();
		emit episodesChanged();
	}
}

QString SeasonInformationService::season() const
{
    QStringList list = seasonId_.split(QChar('|'));
    if(list.size() != 2)
        return "";
    return list[1];
}

Client* SeasonInformationService::client() const
{
	return client_;
}

}
}
}
