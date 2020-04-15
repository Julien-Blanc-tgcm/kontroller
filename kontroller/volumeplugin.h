#ifndef VOLUMEPLUGIN_H
#define VOLUMEPLUGIN_H

#include <QObject>

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

	int status_ = 0;
public:
	explicit VolumePlugin(QObject *parent = nullptr);

	QString name() const;
	int maxVolume() const;
	int minVolume() const;
	int currentVolume() const;
	void updateVolume(int newVolume);
	int volumeStep() const;
	int status() const;

signals:
	void nameChanged(QString);
	void minVolumeChanged(int);
	void maxVolumeChanged(int);
	void currentVolumeChanged(int);
	void volumeStepChanged(int);
	void statusChanged(int status);

public slots:
	void refreshVolume();
	void increaseVolume();
	void decreaseVolume();
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

};

}
}
}

#endif // VOLUMEPLUGIN_H
