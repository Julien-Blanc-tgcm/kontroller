#ifndef STATUSSERVICE_H
#define STATUSSERVICE_H

#include <QObject>
#include <QTimer>

class StatusService : public QObject
{
    Q_OBJECT
public:
    StatusService(QObject* parent = nullptr);

    Q_PROPERTY(int connectionStatus READ connectionStatus NOTIFY connectionStatusChanged)

    int connectionStatus() const;

signals:
    void connectionStatusChanged(int);
private slots:
    void updateConnectionStatus(int connectionStatus);
};

#endif // STATUSSERVICE_H
