#ifndef SYSTEMSERVICE_H
#define SYSTEMSERVICE_H

#include "client.h"

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class SystemService : public QObject
{
	Q_OBJECT
	eu::tgcm::kontroller::Client* client_;

  public:
	explicit SystemService(QObject* parent = nullptr);

	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)

	eu::tgcm::kontroller::Client* client() const;

  signals:
	void clientChanged(eu::tgcm::kontroller::Client* client);

  public slots:
	void shutdownServer();
	void rebootServer();
	void suspendServer();
	void hibernateServer();
	void setClient(eu::tgcm::kontroller::Client* client);
};

#endif // SYSTEMSERVICE_H
}
}
}
