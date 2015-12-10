#ifndef KODIIMAGEPROVIDER_H
#define KODIIMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>

class KodiImageProvider : public QQuickImageProvider
{

public:
    KodiImageProvider();
    ~KodiImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

};

#endif // KODIIMAGEPROVIDER_H
