#include "themeinformation.h"
#include "deviceinformation.h"
#include <QDebug>

#ifndef SAILFISH_TARGET

namespace eu
{
namespace tgcm
{
namespace kontroller
{

int ThemeInformation::horizontalPageMargin() const
{
    return horizontalPageMargin_;
}

void ThemeInformation::setHorizontalPageMargin(int horizontalPageMargin)
{
    if(horizontalPageMargin == horizontalPageMargin_)
        return;
    horizontalPageMargin_ = horizontalPageMargin;
    emit horizontalPageMarginChanged(horizontalPageMargin_);
}

int ThemeInformation::itemSizeSmall() const
{
    return itemSizeSmall_;
}

void ThemeInformation::setItemSizeSmall(int itemSizeSmall)
{
    if(itemSizeSmall_ == itemSizeSmall)
        return;
    itemSizeSmall_ = itemSizeSmall;
    emit itemSizeSmallChanged(itemSizeSmall_);
}

QString ThemeInformation::highlightColor() const
{
    return highlightColor_;
}

void ThemeInformation::setHighlightColor(const QString& highlightColor)
{
    if(highlightColor_ == highlightColor)
        return;
    highlightColor_ = highlightColor;
    emit highlightColorChanged(highlightColor_);
}

int ThemeInformation::fontSizeSmall() const
{
    return fontSizeSmall_;
}

void ThemeInformation::setFontSizeSmall(int fontSizeSmall)
{
    if(fontSizeSmall_ == fontSizeSmall)
        return;
    fontSizeSmall_ = fontSizeSmall;
    emit fontSizeSmallChanged(fontSizeSmall_);
}

int ThemeInformation::paddingSmall() const
{
    return paddingSmall_;
}

void ThemeInformation::setPaddingSmall(int paddingSmall)
{
    if(paddingSmall_ == paddingSmall)
        return;
    paddingSmall_ = paddingSmall;
    emit paddingSmallChanged(paddingSmall_);
}

int ThemeInformation::itemSizeMedium() const
{
    return itemSizeMedium_;
}

void ThemeInformation::setItemSizeMedium(int itemSizeMedium)
{
    if(itemSizeMedium_ == itemSizeMedium)
        return;
    itemSizeMedium_ = itemSizeMedium;
    emit itemSizeMediumChanged(itemSizeMedium_);
}

int ThemeInformation::paddingMedium() const
{
    return paddingMedium_;
}

void ThemeInformation::setPaddingMedium(int paddingMedium)
{
    if(paddingMedium_ == paddingMedium)
        return;
    paddingMedium_ = paddingMedium;
    emit paddingMediumChanged(paddingMedium_);
}

QString ThemeInformation::primaryColor() const
{
    return primaryColor_;
}

void ThemeInformation::setPrimaryColor(const QString& primaryColor)
{
    if(primaryColor_ == primaryColor)
        return;
    primaryColor_ = primaryColor;
    emit primaryColorChanged(primaryColor_);
}

QString ThemeInformation::highlightDimmerColor() const
{
    return highlightDimmerColor_;
}

void ThemeInformation::setHighlightDimmerColor(const QString& highlightDimmerColor)
{
    if(highlightDimmerColor_ == highlightDimmerColor)
        return;
    highlightDimmerColor_ = highlightDimmerColor;
    emit highlightDimmerColorChanged(highlightDimmerColor_);
}

int ThemeInformation::fontSizeExtraSmall() const
{
    return fontSizeExtraSmall_;
}

void ThemeInformation::setFontSizeExtraSmall(int fontSizeExtraSmall)
{
    if(fontSizeExtraSmall_ == fontSizeExtraSmall)
        return;
    fontSizeExtraSmall_ = fontSizeExtraSmall;
    emit fontSizeExtraSmallChanged(fontSizeExtraSmall_);
}

int ThemeInformation::fontSizeNormal() const
{
    return fontSizeNormal_;
}

void ThemeInformation::setFontSizeNormal(int fontSizeNormal)
{
    fontSizeNormal_ = fontSizeNormal;
}

ThemeInformation::ThemeInformation(QObject *parent) : QObject(parent)
{

}

void ThemeInformation::configure(DeviceInformation *information)
{
    setHighlightColor("red");
    setPrimaryColor("blue");
    setHighlightDimmerColor("#666666");
    qreal deviceDpi = information->deviceDpi();
    setBackgroundColor("black");
    if(deviceDpi >= 72 && deviceDpi <= 96)
    {
        setFontSizeSmall(12);
        setHorizontalPageMargin(10);
        setItemSizeSmall(32);
        setPaddingSmall(16);
        setItemSizeMedium(64);
        setPaddingMedium(32);
        setFontSizeExtraSmall(8);
        setFontSizeNormal(16);
    }
    else if(deviceDpi >= 96 && deviceDpi <= 150)
    {
        setFontSizeSmall(20);
        setHorizontalPageMargin(20);
        setItemSizeSmall(64);
        setPaddingSmall(32);
        setItemSizeMedium(128);
        setPaddingMedium(64);
        setFontSizeExtraSmall(16);
        setFontSizeNormal(24);
    }
    else
    {
        setFontSizeSmall(24);
        setHorizontalPageMargin(20);
        setItemSizeSmall(64);
        setPaddingSmall(32);
        setItemSizeMedium(128);
        setPaddingMedium(64);
        setFontSizeExtraSmall(16);
        setFontSizeNormal(30);
    }
    qDebug() << deviceDpi;

}

}
}
}

#endif
