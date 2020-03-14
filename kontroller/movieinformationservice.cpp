#include "movieinformationservice.h"
#include "client.h"
#include "file.h"
#include "videocontrol.h"
#include "utils.h"
#include <algorithm>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
int MovieInformationService::movieId() const
{
	return movieId_;
}

void MovieInformationService::setMovieId(int movieId)
{
	movieId_ = movieId;
	emit movieIdChanged();
}

QString MovieInformationService::title() const
{
	return title_;
}

void MovieInformationService::setTitle(const QString &title)
{
	title_ = title;
	emit titleChanged();
}

QString MovieInformationService::thumbnail() const
{
	return thumbnail_;
}

void MovieInformationService::setThumbnail(const QString &thumbnail)
{
	thumbnail_ = thumbnail;
	emit thumbnailChanged();
}

int MovieInformationService::year() const
{
	return year_;
}

void MovieInformationService::setYear(int year)
{
	year_ = year;
	emit yearChanged();
}

int MovieInformationService::runtime() const
{
	return runtime_;
}

void MovieInformationService::setRuntime(int runtime)
{
	runtime_ = runtime;
	emit runtimeChanged();
}

QString MovieInformationService::genres() const
{
	if(genres_.size() == 0)
		return QString();
	QString ret = genres_[0];
	for(int i = 1; i < genres_.size(); ++i)
	{
		ret += ", " + genres_[i];
	}
	return ret;
}


QString MovieInformationService::plot() const
{
	return plot_;
}

void MovieInformationService::setPlot(const QString &plot)
{
	plot_ = plot;
	emit plotChanged();
}

double MovieInformationService::rating() const
{
	return rating_;
}

void MovieInformationService::setRating(double rating)
{
	rating_ = rating;
	emit ratingChanged();
}

MovieInformationService::MovieInformationService(QObject *parent) : QObject(parent),
    year_(0),
    ctrl_{new VideoControl(this)}
{
}

void MovieInformationService::refresh()
{
	QJsonObject parameters;
	parameters["movieid"] = movieId_;
	QJsonArray properties;
	properties.append(QString("title"));
	properties.append(QString("thumbnail"));
	properties.append(QString("year"));
	properties.append(QString("runtime"));
	properties.append(QString("genre"));
	properties.append(QString("plot"));
	properties.append(QString("rating"));
	properties.append(QString("file"));
	parameters["properties"] = properties;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("VideoLibrary.GetMovieDetails", parameters);
	auto reply = Client::current().send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &MovieInformationService::handleRefresh_);
}

void MovieInformationService::playFile()
{
	auto file = new File(this);
	file->setFiletype("movie");
	file->setType("movie");
	file->setFile(QString::number(movieId_));
	file->setLabel(title());
	ctrl_->playFile(file);
}

void MovieInformationService::handleRefresh_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply)
	{
		auto result = reply->response().result();
		if(!result.isObject())
			return;
		auto detailsTmp = result.toObject().value("moviedetails");
		if(!detailsTmp.isObject())
			return;
		auto details = detailsTmp.toObject();
		setTitle(details.value("title").toString());
		setThumbnail(getImageUrl(details.value("thumbnail").toString()).toString());
		setYear(details.value("year").toInt());
		setRuntime(details.value("runtime").toInt());
		auto genres = details.value("genre").toArray();
		if(!genres.isEmpty())
		{
			for(auto genre : genres)
			{
				if(genre.isString())
					genres_.push_back(genre.toString());
			}
		}
		emit genresChanged();
		setPlot(details.value("plot").toString());
		setRating(details.value("rating").toDouble());
	}
}

}
}
}
