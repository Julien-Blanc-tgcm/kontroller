#include "deviceinformation.h"

#include <QScreen>
#include <QDebug>

class DeviceInformation::Impl
{
public:
    qreal scalingFactor = 1;
};

DeviceType getDeviceType(qreal dpi, qreal height, qreal width)
{
    qreal deviceHeight = height / dpi;
    qreal deviceWidth = width / dpi;

    qreal longEdge = std::max(deviceHeight, deviceWidth);
    //qreal shortEdge = std::min(deviceHeight, deviceWidth);

    if(longEdge < 6) // phone
        return DeviceType::Phone;
    else if(longEdge < 13)
        return DeviceType::Tablet;
    else if(longEdge < 18)
        return DeviceType::Laptop;
    else if(longEdge < 30)
        return DeviceType::Desktop;
    else
        return DeviceType::TV;
}

double getViewDistance(DeviceType type)
{
    switch(type)
    {
        case DeviceType::Phone:
            return 50;
        case DeviceType::Tablet:
            return 50;
        case DeviceType::Laptop:
            return 25;
        case DeviceType::Desktop:
            return 30;
        case DeviceType::TV:
            return 300;
        case DeviceType::Undefined:
            return 50;
    }
    return 50;
}

double getCorrectionFactor(double dpi, double distance)
{
    return (dpi / 96) * distance / getViewDistance(DeviceType::Desktop);
}

DeviceInformation::DeviceInformation(QObject* parent) :
    QObject(parent)
{
}

DeviceInformation::Impl&DeviceInformation::internal()
{
    static Impl i;
    return i;
}

void DeviceInformation::setup(QApplication &app)
{
    QRect rect = app.primaryScreen()->geometry();
    qreal dpi = app.primaryScreen()->physicalDotsPerInch();
    qreal height = rect.height();
    qreal width = rect.width();


    KodiSettings settings;
    DeviceType device;
    if(settings.deviceType() != DeviceType::Undefined)
        device = settings.deviceType();
    else
        device = getDeviceType(dpi, height, width);

    qreal viewDistance = getViewDistance(device);
    qDebug() << dpi << height << width << viewDistance;

    qreal factor = getCorrectionFactor(dpi, viewDistance);
    internal().scalingFactor = factor;
}

qreal DeviceInformation::scalingFactor() const
{
    return internal().scalingFactor;
}

void DeviceInformation::setScalingFactor(qreal scalingFactor)
{
    internal().scalingFactor = scalingFactor;
}
