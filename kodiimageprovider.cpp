#include "kodiimageprovider.h"
#include "kodiclient.h"
#include <QApplication>

KodiImageProvider::KodiImageProvider() : QQuickImageProvider(ImageType::Image, QQuickImageProvider::ForceAsynchronousImageLoading)
{

}

KodiImageProvider::~KodiImageProvider()
{

}

QImage KodiImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    if(id.size() > 0)
    {
        QNetworkAccessManager manager;
        auto val = QByteArray::fromBase64(id.toLatin1());
        auto nid = QString::fromLatin1(val);
        QString url =QString("http://") + KodiClient::current().serverAddress() + ":" + QString::number(KodiClient::current().serverHttpPort()) + "/image/" +
                QUrl::toPercentEncoding(nid);
        auto remoteUrl = QUrl{url};
        auto reply = manager.get(QNetworkRequest(remoteUrl));
        int i = 0;
        QImage ret;
        QObject::connect(reply, &QNetworkReply::finished, reply, [&i, &reply, &ret] {
            QByteArray bytes = reply->readAll();
            ret.loadFromData(bytes);
            i = 1;
        });
        while(i == 0)
            QApplication::processEvents(QEventLoop::AllEvents, 100);
        return ret;
    }
    return QImage();
}

