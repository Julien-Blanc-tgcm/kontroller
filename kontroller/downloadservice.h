#ifndef EU_TGCM_KONTROLLER_DOWNLOADSERVICE_H
#define EU_TGCM_KONTROLLER_DOWNLOADSERVICE_H

#include <QFile>
#include <QNetworkReply>
#include <QObject>
#include <QVector>

#include <memory>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Client;
class ApplicationSettings;

enum class FileType
{
    File,
    Directory
};

struct FileDownload
{
	FileType type;
	QString sourceFile;
	QString destinationPath;
	QFile* output;
	QNetworkReply* reply;
};

/**
 * @brief The DownloadService class handles downloading of files from the remote
 * kodi interface, via the Files.PrepareDownload API
 */
class DownloadService : public QObject
{
    Q_OBJECT
public:
	explicit DownloadService(Client* client, ApplicationSettings* settings, QObject *parent = nullptr);

signals:
    void downloadCompleted(QString file);
    void downloadError(QString file, QString message);
    void downloadStarted(QString file);
public slots:
    void addFile(QString filePath);
    void addFolder(QString folderPath);
private slots:
    void filePathRequestComplete_();
    void downloadCompleted_();
    void startNextDownload_();
    void bytesAvailable_();
    void folderInfoRequestComplete_();
private:
    void startDownloadHttp_(QString httppath);
    void addFile_(QString filePath, QString outputFolder);
    void addFolder_(QString folderPath, QString outputFolder);
	QVector<FileDownload> queue_;
    QString downloadFolder_;

	Client* client_;
	ApplicationSettings* appSettings_;
};

}
}
}

#endif // EU_TGCM_KONTROLLER_DOWNLOADSERVICE_H
