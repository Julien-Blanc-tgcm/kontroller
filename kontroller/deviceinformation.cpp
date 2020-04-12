#include "deviceinformation.h"
#include "applicationsettings.h"

#include <QScreen>
#include <QDebug>
#include "math.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class DeviceInformation::Impl
{
public:
    qreal scalingFactor = 1;
    qreal touchScalingFactor = 1;
    qreal dpi = 96;
    QSizeF size;
    qreal logicalDpi;
    qreal deviceDpi;

    qreal deviceHeight;
    qreal deviceWidth;
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
    else if(longEdge < 24)
        return DeviceType::Desktop;
    else
        return DeviceType::TV;
}

double getViewDistance(DeviceType type)
{
    switch(type)
    {
        case DeviceType::Phone:
            return 40;
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
    double refRatio = 1.0 / getViewDistance(DeviceType::Desktop); // 1cm viewed on a destkop (atan of angle)
    // Now we calculate how to make sure that at the view distance, how many cm will appear just like
    // if they were displayed as 1cm on a desktop (same view angle)
    double deviceCm = distance * refRatio;

    double pixelsOnDevice = deviceCm * pow(dpi, 2) / 2.54;
    double referencePixels = 1.0 * pow(96,2) / 2.54;

    double ratio =  sqrt(pixelsOnDevice / referencePixels);
    // now, text is more lisible if on a high dpi device, so we can reduce its size
    // accordingly
    if(ratio * 12 > 18)
    {
        ratio = ratio * 12 / 18;
    }
    return ratio;
}

double computeTouchCorrectionFactor(double dpi, double scalingFactor)
{
    // our reference is 20 px : this is about 0.529mm on a 96dpi device
    // to be touchable, this must be at least 0.8cm physically (independant of view size)
    double realSize = 20 * scalingFactor / dpi * 2.54;
    if(realSize < 0.8)
        return scalingFactor * 0.8 / realSize;
    else
        return scalingFactor;
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

#ifdef SAILFISH_TARGET
void DeviceInformation::setup(QGuiApplication &app)
#else
void DeviceInformation::setup(QApplication &app)
#endif
{
    QRect rect = app.primaryScreen()->geometry();
    internal().deviceDpi = app.primaryScreen()->physicalDotsPerInch();
    internal().deviceHeight = rect.height();
    internal().deviceWidth = rect.width();
    internal().size = app.primaryScreen()->physicalSize();
    internal().logicalDpi = app.primaryScreen()->logicalDotsPerInch();
//    Settings settings;
//    if(settings.dpi() > 25) // less than 25 is not usable
//        internal().dpi = settings.dpi();
//    else
//        settings.setDpi(internal().deviceDpi);
    update();
}

qreal DeviceInformation::scalingFactor() const
{
    return internal().scalingFactor;
}

qreal DeviceInformation::touchScalingFactor() const
{
    return internal().touchScalingFactor;
}

qreal DeviceInformation::dpi() const
{
    return internal().dpi;
}

QSizeF DeviceInformation::size() const
{
    return internal().size;
}

qreal DeviceInformation::logicalDpi() const
{
    return internal().logicalDpi;
}

qreal DeviceInformation::deviceDpi() const
{
    return internal().deviceDpi;
}

void DeviceInformation::setScalingFactor(qreal scalingFactor)
{
    internal().scalingFactor = scalingFactor;
    emit scalingFactorChanged(scalingFactor);
}

void DeviceInformation::setTouchScalingFactor(qreal scalingFactor)
{
    internal().touchScalingFactor = scalingFactor;
    emit touchScalingFactorChanged(scalingFactor);
}

void DeviceInformation::update()
{
    DeviceType device;
//    Settings settings;
//    internal().dpi = settings.dpi();
//    if(internal().dpi <= 25 || internal().dpi > 3000) // obviously wrong value
//    {
//        internal().dpi = internal().deviceDpi;
//        settings.setDpi(internal().dpi);
//    }
//    if(settings.deviceType() != static_cast<int>(DeviceType::Undefined))
//        device = static_cast<DeviceType>(settings.deviceType());
//    else
        device = getDeviceType(internal().deviceDpi, internal().deviceHeight, internal().deviceWidth);

    qreal viewDistance = getViewDistance(device);

    qreal factor = getCorrectionFactor(internal().dpi, viewDistance);
    internal().scalingFactor = factor;
    switch(device)
    {
    case DeviceType::Desktop:
    case DeviceType::Laptop:
    case DeviceType::TV:
    case DeviceType::Undefined:
        setTouchScalingFactor(factor);
        break;
    case DeviceType::Phone:
    case DeviceType::Tablet:
    default:
        setTouchScalingFactor(computeTouchCorrectionFactor(internal().dpi, factor));
        break;
    }
    emit dpiChanged(internal().dpi);
    emit scalingFactorChanged(internal().scalingFactor);
}


}
}
}
