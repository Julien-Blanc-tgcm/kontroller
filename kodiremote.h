#ifndef KODIREMOTE_H
#define KODIREMOTE_H

#include <QObject>

class KodiRemote : public QObject
{
    Q_OBJECT
public:
    explicit KodiRemote(QObject *parent = 0);

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
};

#endif // KODIREMOTE_H
