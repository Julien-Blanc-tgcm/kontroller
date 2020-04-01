#ifndef KODIVOLUMEPLUGIN_H
#define KODIVOLUMEPLUGIN_H

#include "volumeplugin.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{
/**
 * @brief The VolumePlugin class is the base class for all volume plugins. Volume plugins shall provide a set of method to query
 * the current volume, the min/max volumes and the volume step (ie, what a button press should increment)
 */
class KodiVolumePlugin : public VolumePlugin
{
	Q_OBJECT
public:
	explicit KodiVolumePlugin(QObject *parent = nullptr);

protected:
	QString realName_() const override;
	int maxVolume_() const override;
	int minVolume_() const override;
	int currentVolume_() const override;
	void updateVolume_(int value) override;
	int volumeStep_() const override;
	void refreshVolume_() override;
	void increaseVolume_();
	void decreaseVolume_();
private:
	int currentVolumeStored_ = -1;
};

}
}
}

#endif // VOLUMEPLUGIN_H
