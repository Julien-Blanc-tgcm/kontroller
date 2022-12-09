import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0

Dialog {
    property string serverUuid: ""
    property bool newServer: false
    KodiServiceDiscovery {
        id: serviceDiscovery
    }

    function selectingServer__()
    {
        return newServer && !__serverSelected
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: theCol.height
        DialogHeader
        {}
        Column {
            id:theCol
            spacing: Theme.paddingSmall
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:parent.top

            PageHeader {
                title: qsTr("Server Settings")
            }

            BusyIndicator {
                running: newServer && serviceDiscovery.discovering && !__serverSelected
                size: BusyIndicatorSize.Large
                anchors.horizontalCenter: parent.horizontalCenter
                visible: running
            }
            Label {
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("Discovering servers…");
                wrapMode: Text.Wrap
                visible: newServer && serviceDiscovery.discovering && !__serverSelected
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
            }
            Label {
                text: qsTr("Click one of these discovered servers to select it:")
                visible: newServer && !__serverSelected && serviceDiscovery.servers.length >= 1
                wrapMode: Text.Wrap
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
            }

            Label {
                text: qsTr("No server found. Make sure your device has Wi-Fi turned on and that Kodi is running on the same network.")
                wrapMode: Text.Wrap
                visible: newServer && !__serverSelected && !serviceDiscovery.discovering && serviceDiscovery.servers.length === 0
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
            }

            SilicaListView {
                visible: selectingServer__()
                width:parent.width
                height: serviceDiscovery.servers.length * Theme.itemSizeMedium
                model: serviceDiscovery.servers
                delegate: ListItem {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: Theme.itemSizeMedium
                    DetailItem {
                        label: model.modelData.hostname
                        value: model.modelData.address
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    onClicked: {
                        selectServer(model.modelData)
                    }
                }
            }
            // spacer
            Item {
                height: Theme.paddingLarge
                width: 1
                visible: selectingServer__()
            }

            Label {
                Icon {
                    source:"image://theme/icon-m-add"
                    id: theIcon
                    anchors.verticalCenter: parent.verticalCenter
                    x: (label.width - label.contentWidth - width) / 2
                }
                id:label
                text: qsTr("Enter details manually")
                leftPadding: theIcon.width
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
                anchors.left: parent.left
                anchors.right: parent.right
                MouseArea {
                    anchors.fill:parent
                    onClicked: __serverSelected = true
                }
                visible: selectingServer__()
            }

            TextField {
                id:serverName
                text: appSettings.server(serverUuid).name
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                label:qsTr("Server name");
                placeholderText: qsTr("Server name")
                visible: !selectingServer__()
                EnterKey.enabled: text.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: serverAddress.focus = true
            }

            TextField {
                id: serverAddress
                text: appSettings.server(serverUuid).serverAddress
                anchors.left: parent.left;
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
                label:qsTr("Server address");
                placeholderText: qsTr("Server address");
                visible: !selectingServer__()
                EnterKey.enabled: text.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: serverLogin.focus = true
            }

            TextField {
                id: serverLogin
                text: appSettings.server(serverUuid).login
                anchors.left: parent.left;
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                inputMethodHints: Qt.ImhNoAutoUppercase
                label:qsTr("Username");
                placeholderText: qsTr("Username");
                visible: !selectingServer__()
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: serverPassword.focus = true
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                text: qsTr("(Optional) password for Kodi login. (Stored unencrypted on device.)")
                visible: !selectingServer__()
            }

            TextField {
                id: serverPassword
                text: appSettings.server(serverUuid).password
                anchors.left: parent.left;
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                inputMethodHints: Qt.ImhNoAutoUppercase
                label:qsTr("Password");
                placeholderText: qsTr("Password");
                visible: !selectingServer__()
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: serverPort.focus = true
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                text: qsTr("TCP port for Kodi connection. Change it if you used your \
own value in Kodi. Turn on remote access in kodi to have working notifications.")
                visible: !selectingServer__()
            }

            TextField {
                id: serverPort
                text: appSettings.server(serverUuid).serverPort
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                validator: IntValidator { bottom: 1; top: 65535 }
                inputMethodHints: Qt.ImhDigitsOnly
                label:qsTr("Server port")
                placeholderText: qsTr("Server port")
                visible: !selectingServer__()
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: serverHttpPort.focus = true
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                text: qsTr("HTTP port for Kodi connection. \"8080\" is fine unless you set up \
something else in Kodi. Turn on \"HTTP remote access\" in Kodi for it to work.")
                visible: !selectingServer__()
            }
            TextField {
                id : serverHttpPort
                text: appSettings.server(serverUuid).serverHttpPort
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                validator: IntValidator{ bottom:1; top:65535}
                inputMethodHints: Qt.ImhDigitsOnly
                label:qsTr("Web port")
                placeholderText: qsTr("Web port")
                visible: !selectingServer__()
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: serverHttpPort.focus = false
            }
            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                text: qsTr("Ignore the wi-fi status, try to connect to the server even if the wifi is not connected \
(useful if you connect via another type of connection, such as a VPN).")
                visible: !selectingServer__()
            }
            TextSwitch {
                id:chkIgnoreWifi
                text:qsTr("Ignore Wi-Fi status")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                Component.onCompleted: {
                    checked = appSettings.server(serverUuid).ignoreWifiStatus
                }
            }
            SectionHeader
            {
                text: qsTr("Server features")
                visible: !selectingServer__()
            }

            TextSwitch {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                text: qsTr("Power off");
                id: poweroffSupported
                checked: appSettings.server(serverUuid).poweroffEnabled
                description: qsTr("Keep option in server management pulley menu. On by default.")
                visible: !selectingServer__()
            }

            TextSwitch {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                text: qsTr("Reboot");
                id: rebootSupported
                checked: appSettings.server(serverUuid).rebootEnabled
                description: qsTr("Keep option in server management pulley menu. On by default.")
                visible: !selectingServer__()
            }

            TextSwitch {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                text: qsTr("Hibernate");
                id: hibernateSupported
                checked: appSettings.server(serverUuid).hibernateEnabled
                description: qsTr("If unchecked, the option will not appear in the server management pulley menu. Off by default.")
                visible: !selectingServer__()
            }

            TextSwitch {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                text: qsTr("Suspend");
                id: suspendSupported
                checked: appSettings.server(serverUuid).suspendEnabled
                description: qsTr("If unchecked, the option will not appear in the server management pulley menu. Off by default.")
                visible: !selectingServer__()
            }

            ComboBox {
                id: serverWakeUpPlugin
                anchors.left: parent.left
                anchors.right: parent.right
                label: qsTr("WakeUp plugin")
                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("None")
                    }
                    MenuItem {
                        text: qsTr("Wake-on-LAN")
                        onClicked: wakeUpMacAddress.focus = true
                    }
                }
                Component.onCompleted: {
                    if(appSettings.server(serverUuid).wakeUpPluginName === "WolWakeUp")
                        currentIndex = 1;
                    else
                        currentIndex = 0;
                }
                visible: !selectingServer__()
                onCurrentItemChanged: {
                    if(currentIndex == 1)
                    {
                        refreshMacAddress()
                    }
                }
            }
            LinkedLabel {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                plainText: qsTr("Wake-on-LAN usually needs to be set up on the server, as per
https://github.com/Julien-Blanc-tgcm/kontroller/blob/master/README.md#WakeOnLan")
                visible: serverWakeUpPlugin.currentIndex === 1 && !selectingServer__()
            }

            TextField {
                id : wakeUpMacAddress
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
                validator: RegExpValidator { regExp: /^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$/ }
                label:qsTr("Device MAC address")
                placeholderText: qsTr("Device MAC address")
                visible: serverWakeUpPlugin.currentIndex === 1 && !selectingServer__()
                Component.onCompleted: {
                    if(appSettings.server(serverUuid) && appSettings.server(serverUuid).wakeUpPluginName === "WolWakeUp")
                        text = appSettings.server(serverUuid).wakeUpPluginParameters.macAddress;
                    else
                        text = ""
                }
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: wakeUpMacPort.focus = true
            }
            TextField {
                id : wakeUpMacPort
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
                validator: IntValidator{ bottom:1; top:65535}
                label:qsTr("Wake up port")
                placeholderText: qsTr("Wake up port")
                visible: serverWakeUpPlugin.currentIndex === 1 && !selectingServer__()
                Component.onCompleted: {
                    if(appSettings.server(serverUuid) && appSettings.server(serverUuid).wakeUpPluginName === "WolWakeUp")
                        text = appSettings.server(serverUuid).wakeUpPluginParameters.port;
                    else
                        text = 9; // default value
                }
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: wakeUpMacPort.focus = false
            }

            SectionHeader
            {
                text: qsTr("Volume handling")
                visible: !selectingServer__()
            }
            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Here you can change the way you adjust the volume. By default, kodi internal volume is controlled. \
If you have an external amplifier, you can select and configure the corresponding plugin so that the amplifier volume is \
controlled by the remote instead")
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
                visible: !selectingServer__()
            }
            ComboBox {
                id: serverVolumePlugin
                anchors.left: parent.left
                anchors.right: parent.right
                label: qsTr("Volume plugin")
                menu: ContextMenu {
                    MenuItem {
                        text: qsTr("Kodi")
                    }
                    MenuItem {
                        text: qsTr("miniDSP")
                        onClicked: miniDSPAddress.focus = true
                    }
                }
                Component.onCompleted: {
                    if(appSettings.server(serverUuid).volumePluginName === "Minidsp") // not a displayed value, not cased correctly, but keep it for compatibility
                        currentIndex = 1;
                    else
                        currentIndex = 0;
                }
                visible: !selectingServer__()
            }
            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                text: qsTr("IP address of the miniDSP device")
                visible: serverVolumePlugin.currentIndex === 1 && !selectingServer__()
            }
            TextField {
                id : miniDSPAddress
                text: appSettings.server(serverUuid) && appSettings.server(serverUuid).volumePluginName === "Minidsp"?
                    appSettings.server(serverUuid).volumePluginParameters.address:""
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
                label:qsTr("miniDSP IP address")
                placeholderText: qsTr("miniDSP IP address")
                visible: serverVolumePlugin.currentIndex === 1 && !selectingServer__()
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: miniDSPAddress.focus = false
            }

/*            TextSwitch {
                id: serverHasZones
                text:qsTr("Use multiples zones")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                checked:settings.server(serverUuid).hasZones
                onCheckedChanged: {
                    settings.server(serverUuid).setHasZones(serverHasZones.checked);
                    pushOrPullZonePage();
                }
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                color:Theme.highlightColor
                text:qsTr("Zones uses different audio output. Go to next page to learn for current zones.")
                wrapMode: Text.WordWrap
            } */
        }
    }

    onAccepted: {
        appSettings.server(serverUuid).setName(serverName.text)
        appSettings.server(serverUuid).setPassword(serverPassword.text);
        appSettings.server(serverUuid).setServerAddress(serverAddress.text);
        appSettings.server(serverUuid).setServerHttpPort(serverHttpPort.text);
        appSettings.server(serverUuid).setServerPort(serverPort.text);
        appSettings.server(serverUuid).setLogin(serverLogin.text);

        appSettings.server(serverUuid).setRebootEnabled(rebootSupported.checked);
        appSettings.server(serverUuid).setPoweroffEnabled(poweroffSupported.checked);
        appSettings.server(serverUuid).setSuspendEnabled(suspendSupported.checked);
        appSettings.server(serverUuid).setHibernateEnabled(hibernateSupported.checked);
        appSettings.server(serverUuid).setIgnoreWifiStatus(chkIgnoreWifi.checked);

        if(serverVolumePlugin.currentIndex === 0)
            appSettings.server(serverUuid).setVolumePluginName("Kodi");
        else
        {
            appSettings.server(serverUuid).setVolumePluginName("Minidsp");
            appSettings.server(serverUuid).setVolumePluginParameters({"address": miniDSPAddress.text});
        }
        if(serverWakeUpPlugin.currentIndex === 0)
            appSettings.server(serverUuid).setWakeUpPluginName("None");
        else
        {
            appSettings.server(serverUuid).setWakeUpPluginName("WolWakeUp");
            appSettings.server(serverUuid).setWakeUpPluginParameters({"macAddress": wakeUpMacAddress.text,
                                                                      "port": parseInt(wakeUpMacPort.text)})
        }

        appSettings.save();
        if(appClient.server.uuid === serverUuid)
        { // we are using this server, we need to refresh it
            appClient.refresh();
        }
    }

    onRejected: {
        if(newServer)
        {
            appSettings.deleteServer(serverUuid);
        }
    }

    Component.onCompleted: {
        if(newServer)
            serviceDiscovery.startDiscovery()
    }

    MacAddressFinder {
        id: macAddressFinder
    }

    function refreshMacAddress()
    {
        if(appSettings.server(serverUuid))
        {
            var mac = macAddressFinder.readMacAddress(appSettings.server(serverUuid).serverAddress);
            if(mac && mac.length > 0)
            {
                wakeUpMacAddress.text = mac;
            }
        }
    }

    function pushOrPullZonePage()
    {
        console.log("pushhere");
//        if(chkIgnoreWifi.checked)
//        {
//            console.log("pushAttached");
//            zones = pageStack.pushAttached(Qt.resolvedUrl("ManageZones.qml"));
//        }
//        else if(zones)
//            pageStack.popAttached(zones);
    }

    property bool __serverSelected: false

    function selectServer(item) {
        serverHttpPort.text = item.port
        serverAddress.text = item.address
        serverName.text = item.hostname
        __serverSelected = true;
    }
}
