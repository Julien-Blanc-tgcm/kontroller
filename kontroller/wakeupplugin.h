#ifndef WAKEUPPLUGIN_H
#define WAKEUPPLUGIN_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

/**
 * @brief The WakeUpPlugin class is the base class (abstract class) for all wake up
 * plugins
 */
class WakeUpPlugin : public QObject
{
	Q_OBJECT

  public:
	explicit WakeUpPlugin(QObject* parent = nullptr);

	Q_PROPERTY(QString name READ name NOTIFY nameChanged)

	virtual QString name() const = 0;

  signals:
	void nameChanged(QString name);

  public slots:
	void wakeUp();

  protected:
	virtual void wakeUp_() = 0;
};

}
}
}


#endif // WAKEUPPLUGIN_H
