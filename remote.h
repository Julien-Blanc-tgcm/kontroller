#ifndef EU_TGCM_KONTROLLER_REMOTE_H
#define EU_TGCM_KONTROLLER_REMOTE_H

#include <QObject>
namespace eu
{
namespace tgcm
{
namespace kontroller
{

class Remote : public QObject
{
    Q_OBJECT
public:
    explicit Remote(QObject *parent = 0);

signals:

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
};

}
}
}
#endif // EU_TGCM_KONTROLLER_REMOTE_H
