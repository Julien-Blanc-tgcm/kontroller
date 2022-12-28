#ifndef MARANTZAVRVOLUMEPLUGIN_H
#define MARANTZAVRVOLUMEPLUGIN_H

#include "volumeplugin.h"

#include "kodivolumeplugin.h"

#include <eu/tgcm/avrcontrol/AvrDevice.hpp>

#include <QVariantMap>
#include <QVector>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
class Client;
/**
 * @brief Implementation of a volume plugin for setting the volume on an avr device using the
 * marantz rs protocol.
 *
 * the AVR has some strange behaviour on the max volume. For the sake of this plugin, the max
 * volume has been set to 900. This is not exactly true, but should be ok for most AVRs.
 */
class MarantzAvrVolumePlugin : public VolumePlugin
{
	Q_OBJECT
  public:
	explicit MarantzAvrVolumePlugin(Client *parent);

	Q_PROPERTY(QString ipAddress READ ipAddress WRITE setIpAddress NOTIFY ipAddressChanged)
	Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
	/**
	 * Force the kodi volume at a specified level on connection (true to force)
	 */
	Q_PROPERTY(bool forceKodiVolume READ forceKodiVolume WRITE setForceKodiVolume NOTIFY forceKodiVolumeChanged)
	/**
	 * The level at which to force the volume
	 */
	Q_PROPERTY(int kodiForcedVolume READ kodiForcedVolume WRITE setKodiForcedVolume NOTIFY kodiForcedVolumeChanged)

	/**
	 * Force a specific source on the avr device (true to force)
	 */
	Q_PROPERTY(bool updateSource READ updateSource WRITE setUpdateSource NOTIFY updateSourceChanged)
	/**
	 * The source to force-set (will be forced set on each playing startup)
	 */
	Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
	/**
	 * The current source of the avr (useful at config step)
	 */
	Q_PROPERTY(QString currentSource READ currentSource NOTIFY currentSourceChanged)

	Q_PROPERTY(QStringList availableSources READ availableSources NOTIFY availableSourcesChanged)

	QString ipAddress() const;

	bool connected() const;

	static QString static_name();

	QString currentSource() const;

	QString source() const;

	void setSource(QString source);

	bool updateSource() const;

	void setUpdateSource(bool update);

	int kodiForcedVolume() const;

	void setKodiForcedVolume(int volume);

	bool forceKodiVolume() const;

	void setForceKodiVolume(bool force);

	void setParameters(QVariantMap const& parameters);

	const QStringList &availableSources() const;

  public slots:
	void setIpAddress(QString ipaddress);

	void setConnected(bool connected);

  signals:
	void ipAddressChanged(QString ipaddress);

	void connectedChanged(bool connected);

	void sourceChanged();

	void currentSourceChanged();

	void updateSourceChanged();

	void kodiForcedVolumeChanged();

	void forceKodiVolumeChanged();

	void availableSourcesChanged();

  protected:
	QString realName_() const override;
	int maxVolume_() const override;
	int minVolume_() const override;
	int currentVolume_() const override;
	void updateVolume_(int newVolume) override;
	int volumeStep_() const override;
	void refreshVolume_() override;
	bool valueValid_() const override;
	QString displayValue_() const override;
	QString formatVolume_(int value) const override;
	void setMuted_(bool muted) override;
	bool muted_() const override;
  private:
	eu::tgcm::avrremote::AvrDevice* device_ = nullptr;
	KodiVolumePlugin* kodiVolumePlugin_ = nullptr;

	QString ipAddress_;

	bool connected_ = false;

	bool forceKodiVolume_ = false;
	int kodiVolume_ = 100;

	bool updateSource_ = false;
	QString source_;

  private slots:
	void handleConnected_();
	void updateAtStartup_(int playerId, int speed);
};

} // namespace kontroller
} // namespace tgcm
} // namespace eu

#endif // MARANTZAVRVOLUMEPLUGIN_H
