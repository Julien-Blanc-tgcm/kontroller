#ifndef THEMEINFORMATION_H
#define THEMEINFORMATION_H

#ifndef SAILFISH_TARGET
// not used for sailfish

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class DeviceInformation;

class ThemeInformation : public QObject
{
    Q_OBJECT
    int horizontalPageMargin_;
    int itemSizeSmall_;
    QString highlightColor_;
    int fontSizeSmall_;
    int paddingSmall_;
    int itemSizeMedium_;
    int paddingMedium_;
    QString primaryColor_;
    QString highlightDimmerColor_;
    int fontSizeExtraSmall_;
    int fontSizeNormal_;

public:
    explicit ThemeInformation(QObject *parent = 0);
    Q_PROPERTY(int horizontalPageMargin READ horizontalPageMargin WRITE setHorizontalPageMargin NOTIFY horizontalPageMarginChanged)
    Q_PROPERTY(int itemSizeSmall READ itemSizeSmall WRITE setItemSizeSmall NOTIFY itemSizeSmallChanged)
    Q_PROPERTY(QString highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)
    Q_PROPERTY(int fontSizeSmall READ fontSizeSmall WRITE setFontSizeSmall NOTIFY fontSizeSmallChanged)
    Q_PROPERTY(int paddingSmall READ paddingSmall WRITE setPaddingSmall NOTIFY paddingSmallChanged)
    Q_PROPERTY(int itemSizeMedium READ itemSizeMedium WRITE setItemSizeMedium NOTIFY itemSizeMediumChanged)
    Q_PROPERTY(int paddingMedium READ paddingMedium WRITE setPaddingMedium NOTIFY paddingMediumChanged)
    Q_PROPERTY(QString primaryColor READ primaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(QString highlightDimmerColor READ highlightDimmerColor WRITE setHighlightDimmerColor NOTIFY highlightDimmerColorChanged)
    Q_PROPERTY(int fontSizeExtraSmall READ fontSizeExtraSmall WRITE setFontSizeExtraSmall NOTIFY fontSizeExtraSmallChanged)
    Q_PROPERTY(int fontSizeNormal READ fontSizeNormal WRITE setFontSizeNormal NOTIFY fontSizeNormalChanged)

    int horizontalPageMargin() const;

    int itemSizeSmall() const;

    QString highlightColor() const;

    int fontSizeSmall() const;

    int paddingSmall() const;

    int itemSizeMedium() const;

    int paddingMedium() const;

    QString primaryColor() const;

    QString highlightDimmerColor() const;

    int fontSizeExtraSmall() const;

    int fontSizeNormal() const;

signals:
    void horizontalPageMarginChanged(int horizontalPageMargin);
    void itemSizeSmallChanged(int itemSizeSmall);
    void highlightColorChanged(QString highlightColor);
    void fontSizeSmallChanged(int fontSizeSmall);
    void paddingSmallChanged(int paddingSmall);
    void itemSizeMediumChanged(int itemSizeMedium);
    void paddingMediumChanged(int paddingMedium);
    void primaryColorChanged(QString primaryColor);
    void highlightDimmerColorChanged(QString highlightDimmerColor);
    void fontSizeExtraSmallChanged(int fontSizeExtraSmall);
    void fontSizeNormalChanged(int fontSizeNormal);

public slots:
    void setHorizontalPageMargin(int horizontalPageMargin);
    void setItemSizeSmall(int itemSizeSmall);
    void setHighlightColor(const QString& highlightColor);
    void setFontSizeSmall(int fontSizeSmall);
    void setPaddingSmall(int paddingSmall);
    void setItemSizeMedium(int itemSizeMedium);
    void setPaddingMedium(int paddingMedium);
    void setPrimaryColor(const QString& primaryColor);
    void setHighlightDimmerColor(const QString& highlightDimmerColor);
    void setFontSizeExtraSmall(int fontSizeExtraSmall);
    void setFontSizeNormal(int fontSizeNormal);
    void configure(eu::tgcm::kontroller::DeviceInformation* information);
};

}
}
}
#endif // SAILFISH_TARGET

#endif // THEMEINFORMATION_H
