#ifndef LIBRARYSERVICE_H
#define LIBRARYSERVICE_H

#include "client.h"

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class LibraryService : public QObject
{
	Q_OBJECT
  public:
	explicit LibraryService(QObject* parent = nullptr);

	~LibraryService() = default;

	LibraryService(LibraryService const&) = delete;
	LibraryService(LibraryService&&) = delete;
	LibraryService& operator=(LibraryService const&) = delete;
	LibraryService& operator=(LibraryService&&) = delete;

	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)

	Q_PROPERTY(int videoLibraryScanProgress READ videoLibraryScanProgress WRITE setVideoLibraryScanProgress NOTIFY
	               videoLibraryScanProgressChanged)
	Q_PROPERTY(int videoLibraryCleanProgress READ videoLibraryCleanProgress WRITE setVideoLibraryCleanProgress NOTIFY
	               videoLibraryCleanProgressChanged)
	Q_PROPERTY(int audioLibraryScanProgress READ audioLibraryScanProgress WRITE setAudioLibraryScanProgress NOTIFY
	               audioLibraryScanProgressChanged)
	Q_PROPERTY(int audioLibraryCleanProgress READ audioLibraryCleanProgress WRITE setAudioLibraryCleanProgress NOTIFY
	               audioLibraryCleanProgressChanged)

	Client* client() const;
	int videoLibraryScanProgress() const;

	int videoLibraryCleanProgress() const;

	int audioLibraryScanProgress() const;

	int audioLibraryCleanProgress() const;

  public slots:
	void setClient(eu::tgcm::kontroller::Client* client);
	void setVideoLibraryScanProgress(int videoLibraryScanProgress);
	void setVideoLibraryCleanProgress(int videoLibraryCleanProgress);
	void setAudioLibraryScanProgress(int audioLibraryScanProgress);
	void setAudioLibraryCleanProgress(int audioLibraryCleanProgress);

	void refreshVideoLibrary();
	void refreshAudioLibrary();
	void cleanVideoLibrary();
	void cleanAudioLibrary();

  signals:
	void clientChanged(eu::tgcm::kontroller::Client* client);
	void videoLibraryScanProgressChanged(int videoLibraryScanProgress);

	void videoLibraryCleanProgressChanged(int videoLibraryCleanProgress);

	void audioLibraryScanProgressChanged(int audioLibraryScanProgress);

	void audioLibraryCleanProgressChanged(int audioLibraryCleanProgress);

  private:
	eu::tgcm::kontroller::Client* client_ = nullptr;
	int videoLibraryScanProgress_;
	int videoLibraryCleanProgress_;
	int audioLibraryScanProgress_;
	int audioLibraryCleanProgress_;
  private slots:
	void handleRefreshVideoLibraryReply_();
	void handleVideoLibraryScanStarted_();
	void handleVideoLibraryScanFinished_();
	void handleAudioLibraryScanStarted_();
	void handleAudioLibraryScanFinished_();
	void handleVideoLibraryCleanStarted_();
	void handleVideoLibraryCleanFinished_();
	void handleAudioLibraryCleanStarted_();
	void handleAudioLibraryCleanFinished_();
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu
#endif // LIBRARYSERVICE_H
