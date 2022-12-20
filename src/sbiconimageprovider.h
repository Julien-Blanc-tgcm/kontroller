#ifndef SBICONIMAGEPROVIDER_H
#define SBICONIMAGEPROVIDER_H

#include <QQuickImageProvider>

class SBIconImageProvider : public QQuickImageProvider
{
	QStringList paths_;

  public:
	SBIconImageProvider();
	SBIconImageProvider(SBIconImageProvider&&) = delete;
	SBIconImageProvider(SBIconImageProvider const&) = delete;
	SBIconImageProvider& operator=(SBIconImageProvider&&) = delete;
	SBIconImageProvider& operator=(SBIconImageProvider const&) = delete;

	~SBIconImageProvider() noexcept override;
	// QQuickImageProvider interface
  public:
	QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) override;
};

#endif // SBICONIMAGEPROVIDER_H
