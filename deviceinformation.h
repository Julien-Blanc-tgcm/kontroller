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
    Q_PROPERTY(qreal touchScalingFactor READ touchScalingFactor WRITE setTouchScalingFactor NOTIFY touchScalingFactorChanged)
    Q_PROPERTY(qreal dpi READ dpi NOTIFY dpiChanged)

    qreal scalingFactor() const;
    qreal touchScalingFactor() const;
    qreal dpi() const;

    Q_INVOKABLE QSizeF size() const;
    Q_INVOKABLE qreal logicalDpi() const;
    Q_INVOKABLE qreal deviceDpi() const;

    Q_INVOKABLE void update();
private:
    static Impl& internal();
public:
    void setup(QApplication& app);
public slots:
    void setScalingFactor(qreal scalingFactor);
    void setTouchScalingFactor(qreal scalingFactor);

signals:
    void scalingFactorChanged(qreal scalingFactor);
    void touchScalingFactorChanged(qreal touchScalingFactor);
    void dpiChanged(qreal dpi);

};

#endif // DEVICEINFORMATION_H
