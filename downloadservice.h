#ifndef EU_TGCM_KONTROLLER_DOWNLOADSERVICE_H
#define EU_TGCM_KONTROLLER_DOWNLOADSERVICE_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

/**
 * @brief The DownloadService class handles downloading of files from the remote
 * kodi interface, via the Files.PrepareDownload API
 */
class DownloadService : public QObject
{
    Q_OBJECT
public:
    explicit DownloadService(QObject *parent = nullptr);

signals:
    void downloadCompleted(QString file);
    void downloadError(QString file, QString message);
public slots:
    void addFile(QString filePath);

private slots:
    void filePathRequestComplete_();
    void downloadCompleted_();
private:
    void startDownloadHttp_(QString httppath);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_DOWNLOADSERVICE_H
