#ifndef KODISERVICEDISCOVERY_H
#define KODISERVICEDISCOVERY_H

#include <QObject>

#include <mdns/sbzeroconfservicediscovery.h>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class KodiServiceDiscovery : public QObject
{
	Q_OBJECT
	QVariantList servers_;
	bool discovering_;

	SBZeroConfServiceDiscovery* discovery_;
public:
	explicit KodiServiceDiscovery(QObject *parent = nullptr);
	~KodiServiceDiscovery() noexcept = default;
	KodiServiceDiscovery(KodiServiceDiscovery&&) noexcept = delete;
	KodiServiceDiscovery(KodiServiceDiscovery const&) = delete;
	KodiServiceDiscovery& operator=(KodiServiceDiscovery&&) noexcept = delete;
	KodiServiceDiscovery& operator=(KodiServiceDiscovery const&) = delete;

	Q_PROPERTY(QVariantList servers READ servers NOTIFY serversChanged)
	Q_PROPERTY(bool discovering READ discovering NOTIFY discoveringChanged)

	QVariantList servers() const;

	bool discovering() const;

  signals:
	void discoveryFinished();
	void serversChanged(QVariantList servers);

	void discoveringChanged(bool discovering);

  public slots:
	void startDiscovery();
	void abortDiscovery();

  private slots:
	void setDiscovering_(bool discovering);
	void handleDiscoveryFinished_();
	void addServer_(SBZeroConfServiceRecord);

  private:
	QNetworkInterface findRelevantInterface_();
};

} // kontroller
} // tgcm
} // eu

#endif // KODISERVICEDISCOVERY_H
