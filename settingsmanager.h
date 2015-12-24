#ifndef EU_TGCM_KONTROLLER_SETTINGSMANAGER_H
#define EU_TGCM_KONTROLLER_SETTINGSMANAGER_H

#include <QString>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

enum class DeviceType {
    Undefined,
    Phone,
    Tablet,
    Laptop,
    Desktop,
    TV
};

/**
 * @brief The SettingsManager class is a singleton class used to store and update
 * application parameters
 */
class SettingsManager
{
    SettingsManager();
private:
    QString serverAddress_;
    int serverPort_;
    bool musicFileBrowsing_;
    bool videosFileBrowsing_;
    bool useHttpInterface_;
    DeviceType deviceType_;
    int dpi_;
    bool ignoreWifiStatus_;
public:
    /**
     * @brief instance returns a reference to the current SettingsManager
     * @return
     */
    static SettingsManager& instance();

    /**
     * @brief serverAddress The address of the server the application connects to
     * (currently, only on server address is supported)
     * @return address of the remote server
     */
    QString serverAddress() const;
    /**
     * @brief serverPort the port the application will connect to. Defaults to 8080
     * @return port of the remote server
     */
    int serverPort() const;
    /**
     * @brief musicFileBrowsing Tells whether music browsing uses file mode or library
     * mode.
     * @return true if music browsing uses file mode, false otherwise
     */
    bool musicFileBrowsing() const;
    /**
     * @brief moviesFileBrowsing Tells whether movies browsing uses file mode or library
     * mode.
     * @return true if movies browsing uses file mode, false otherwise (video file browsing)
     */
    bool videosFileBrowsing() const;

    void setServer(QString address, int port);
    void setMusicFileBrowsing(bool browsing);
    void setVideosFileBrowsing(bool browsing);

    void save();

    bool useHttpInterface() const;
    void setUseHttpInterface(bool useHttpInterface);

    DeviceType deviceType() const;
    void setDeviceType(DeviceType type);

    int dpi() const;
    void setDpi(int dpi);
    bool ignoreWifiStatus() const;
    void setIgnoreWifiStatus(bool ignoreWifiStatus);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_SETTINGSMANAGER_H
