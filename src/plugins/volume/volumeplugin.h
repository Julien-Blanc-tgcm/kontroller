#ifndef VOLUMEPLUGIN_H
#define VOLUMEPLUGIN_H

#include <QObject>
#include <QVariantMap>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Client;

/**
 * @brief The VolumePlugin class is the base class for all volume plugins. Volume plugins shall provide a set of method to query
 * the current volume, the min/max volumes and the volume step (ie, what a button press should increment)
 */
class VolumePlugin : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ name NOTIFY nameChanged)
	Q_PROPERTY(int minVolume READ minVolume NOTIFY minVolumeChanged)
	Q_PROPERTY(int maxVolume READ maxVolume NOTIFY maxVolumeChanged)
	Q_PROPERTY(int currentVolume READ currentVolume NOTIFY currentVolumeChanged)
	Q_PROPERTY(int volumeStep READ volumeStep NOTIFY volumeStepChanged)
	/**
	 * status of the volume plugin, 0 if issue / unconnected, 1 otherwise
	 */
	Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged)

	Q_PROPERTY(QString displayValue READ displayValue NOTIFY currentVolumeChanged)

	/**
	 * Tells whether the current value is valid. Can be used to prevent ui from setting stupid values until
	 * the real value is read
	 */
	Q_PROPERTY(bool valueValid READ valueValid NOTIFY valueValidChanged)

	Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)

	int status_ = 0;

public:
	explicit VolumePlugin(QObject *parent = nullptr);

	QString name() const;
	int maxVolume() const;
	int minVolume() const;
	int currentVolume() const;
	int volumeStep() const;
	int status() const;

	QString displayValue() const;

	bool valueValid() const;

	/**
	 * Returns a displayable value, as with displayValue, but with the given volume. Is used by the slider
	 * to preview the value that will be set
	 */
	Q_INVOKABLE QString formatVolume(int volume) const;

	bool muted() const;

	/**
	 * Returns a newly constructed volume plugin. The type of the plugin depends on its name. The plugin
	 * is configured using the given settings, if applicable
	 */
	static VolumePlugin* getVolumePlugin(Client* client, QString const& name, QVariantMap const& pluginSettings);

signals:
	void nameChanged(QString);
	void minVolumeChanged(int);
	void maxVolumeChanged(int);
	void currentVolumeChanged(int);
	void volumeStepChanged(int);
	void statusChanged(int status);

	void valueValidChanged(bool valueValid);

	void mutedChanged(bool muted);

public slots:
	void refreshVolume();
	void increaseVolume();
	void decreaseVolume();
	void updateVolume(int newVolume);
	void setMuted(bool muted);

protected slots:
	void setStatus(int status);

protected:
	virtual QString realName_() const = 0;
	virtual int maxVolume_() const = 0;
	virtual int minVolume_() const = 0;
	virtual int currentVolume_() const = 0;
	virtual void updateVolume_(int value) = 0;
	virtual int volumeStep_() const = 0;
	virtual void refreshVolume_() = 0;
	virtual void increaseVolume_();
	virtual void decreaseVolume_();
	virtual bool valueValid_() const = 0;
	virtual QString displayValue_() const = 0;
	virtual QString formatVolume_(int volume) const = 0;
	virtual bool muted_() const = 0;
	virtual void setMuted_(bool muted) = 0;

};

} // namespace kontroller
} // namespace tgcm
} // namespace eu

#endif // VOLUMEPLUGIN_H
