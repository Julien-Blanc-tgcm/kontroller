#ifndef DEVICEINFORMATION_H
#define DEVICEINFORMATION_H

#include <QObject>
#include <QApplication>
#include "kodisettings.h"

class DeviceInformation : public QObject
{
    Q_OBJECT

    class Impl;
public:
    DeviceInformation(QObject* parent = nullptr);
    Q_PROPERTY(qreal scalingFactor READ scalingFactor WRITE setScalingFactor NOTIFY scalingFactorChanged)

    qreal scalingFactor() const;

private:
    static Impl& internal();
public:
    void setup(QApplication& app);
public slots:
    void setScalingFactor(qreal scalingFactor);

signals:
    void scalingFactorChanged(qreal scalingFactor);
};

#endif // DEVICEINFORMATION_H
