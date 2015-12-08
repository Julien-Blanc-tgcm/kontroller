#ifndef KODISETTINGSMANAGER_H
#define KODISETTINGSMANAGER_H

#include <QString>


enum class DeviceType {
    Undefined,
    Phone,
    Tablet,
    Laptop,
    Desktop,
    TV
};

/**
 * @brief The KodiSettingsManager class is a singleton class used to store and update
 * application parameters
 */
class KodiSettingsManager
{
    KodiSettingsManager();
private:
    QString m_serverAddress;
    int m_serverPort;
    bool m_musicFileBrowsing;
    bool m_videosFileBrowsing;
    bool m_useHttpInterface;
    DeviceType deviceType_;
    int dpi_;
public:
    /**
     * @brief instance returns a reference to the current KodiSettingsManager
     * @return
     */
    static KodiSettingsManager& instance();

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
};

#endif // KODISETTINGSMANAGER_H
