#include "movieinformationservice.h"
#include "kodiclient.h"

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

QString MovieInformationService::genre() const
{
    return genre_;
}

void MovieInformationService::setGenre(const QString &genre)
{
    genre_ = genre;
    emit genreChanged();
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
    year_(0)
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
    parameters["properties"] = properties;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("VideoLibrary.GetMovieDetails", parameters);
    auto reply = KodiClient::current().send(message);
    connect(reply, &QJsonRpcServiceReply::finished, this, &MovieInformationService::handleRefresh_);
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
        auto b = details.value("thumbnail").toString().toUtf8();
        setThumbnail(QString::fromLatin1(b.toBase64()));
        setYear(details.value("year").toInt());
        setRuntime(details.value("runtime").toInt());
        setGenre(details.value("genre").toString());
        setPlot(details.value("plot").toString());
        setRating(details.value("rating").toDouble());
        qDebug() << details;
    }
}
