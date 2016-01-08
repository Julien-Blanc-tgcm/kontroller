#ifndef EU_TGCM_KONTROLLER_SETTINGSMANAGER_H
#define EU_TGCM_KONTROLLER_SETTINGSMANAGER_H

#include <QString>

#include "server.h"
#include <memory>

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
    std::vector<std::unique_ptr<Server> > servers_;
/*    QString serverAddress_;
    int serverPort_;
    bool musicFileBrowsing_;
    bool videosFileBrowsing_;
    bool useHttpInterface_;
    DeviceType deviceType_;
    int dpi_;
    int serverHttpPort_; */
    bool ignoreWifiStatus_;
public:
    /**
     * @brief instance returns a reference to the current SettingsManager
     * @return
     */
    static SettingsManager& instance();

    std::vector<std::unique_ptr<Server>>& servers();

    Server* server(QString const& name);

    void save();

/*    bool useHttpInterface() const;
    void setUseHttpInterface(bool useHttpInterface);

    DeviceType deviceType() const;
    void setDeviceType(DeviceType type);

    int dpi() const;
    void setDpi(int dpi);*/
    bool ignoreWifiStatus() const;
    void setIgnoreWifiStatus(bool ignoreWifiStatus);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_SETTINGSMANAGER_H
