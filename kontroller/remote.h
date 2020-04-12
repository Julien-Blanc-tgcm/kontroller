#ifndef EU_TGCM_KONTROLLER_REMOTE_H
#define EU_TGCM_KONTROLLER_REMOTE_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Client;

class Remote : public QObject
{
	Q_OBJECT
	eu::tgcm::kontroller::Client* client_;

public:
	explicit Remote(QObject *parent = 0);
	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	eu::tgcm::kontroller::Client* client() const;

signals:
	void clientChanged(eu::tgcm::kontroller::Client* client);

public slots:
	void back();
	void contextMenu();
	void down();
	//void executeAction();
	void home();
	void info();
	void left();
	void right();
	void select();
	void showCodec();
	void showOSD();
	void up();
	void volumeUp();
	void volumeDown();
	void setClient(eu::tgcm::kontroller::Client* client);
};

}
}
}
#endif // EU_TGCM_KONTROLLER_REMOTE_H
