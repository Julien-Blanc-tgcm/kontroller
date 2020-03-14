#ifndef EU_TGCM_KONTROLLER_MOVIEINFORMATIONSERVICE_H
#define EU_TGCM_KONTROLLER_MOVIEINFORMATIONSERVICE_H

#include <QObject>
#include <QStringList>
namespace eu
{
namespace tgcm
{
namespace kontroller
{

class VideoControl;

class MovieInformationService : public QObject
{
    Q_OBJECT
private:
    int movieId_;
    QString title_;
    QString thumbnail_;
    int year_;
    int runtime_;
    QStringList genres_;
    QString plot_;
    double rating_;
	VideoControl* ctrl_;
public:
    explicit MovieInformationService(QObject *parent = 0);

    Q_PROPERTY(int movieId READ movieId WRITE setMovieId NOTIFY movieIdChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail NOTIFY thumbnailChanged)
    Q_PROPERTY(int year READ year WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(int runtime READ runtime WRITE setRuntime NOTIFY runtimeChanged)
    Q_PROPERTY(QString genres READ genres NOTIFY genresChanged)
    Q_PROPERTY(QString plot READ plot WRITE setPlot NOTIFY plotChanged)
    Q_PROPERTY(double rating READ rating WRITE setRating NOTIFY ratingChanged)

    int movieId() const;
    void setMovieId(int movieId);

    QString title() const;
    void setTitle(const QString &title);

    QString thumbnail() const;
    void setThumbnail(const QString &thumbnail);

    int year() const;
    void setYear(int year);

    int runtime() const;
    void setRuntime(int runtime);

    QString genres() const;

    QString plot() const;
    void setPlot(const QString &plot);

    double rating() const;
    void setRating(double rating);

signals:
    void movieIdChanged();
    void titleChanged();
    void thumbnailChanged();
    void yearChanged();
    void runtimeChanged();
    void genresChanged();
    void plotChanged();
    void ratingChanged();
public slots:
    void refresh();
    void playFile();
private slots:
    void handleRefresh_();
};

}
}
}

#endif // EU_TGCM_KONTROLLER_MOVIEINFORMATIONSERVICE_H
