#include "wakeupplugin.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

WakeUpPlugin::WakeUpPlugin(QObject* parent) : QObject(parent)
{
}

void WakeUpPlugin::wakeUp()
{
	wakeUp_();
}

}
}
}

