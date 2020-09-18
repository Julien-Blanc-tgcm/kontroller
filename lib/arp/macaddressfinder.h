#ifndef EU_TGCM_ARP_MACADDRESSFINDER_H
#define EU_TGCM_ARP_MACADDRESSFINDER_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace arp
{

class MacAddressFinder : public QObject // to be instantiable from qml
{
	Q_OBJECT
  public:
	MacAddressFinder(QObject* parent = nullptr);
	~MacAddressFinder() = default;

	Q_INVOKABLE QString readMacAddress(QString ipAddress);

  private:
	QString extractMacAddress_(QString arpLine);
};

}
}
}

#endif // EU_TGCM_ARP_MACADDRESSFINDER_H
