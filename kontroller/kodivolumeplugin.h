#ifndef KODIVOLUMEPLUGIN_H
#define KODIVOLUMEPLUGIN_H

#include "volumeplugin.h"

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
class KodiVolumePlugin : public VolumePlugin
{
	Q_OBJECT

	Client* client_;
public:
	explicit KodiVolumePlugin(Client *parent = nullptr);

	static QString static_name();
protected:
	QString realName_() const override;
	int maxVolume_() const override;
	int minVolume_() const override;
	int currentVolume_() const override;
	void updateVolume_(int newVolume) override;
	int volumeStep_() const override;
	void refreshVolume_() override;
	void increaseVolume_() override;
	void decreaseVolume_() override;
	bool valueValid_() const override;
	QString displayValue_() const override;
	QString formatVolume_(int value) const override;
private:
	int currentVolumeStored_ = -1;
private slots:
	void volumeReply_();

};

}
}
}

#endif // VOLUMEPLUGIN_H
