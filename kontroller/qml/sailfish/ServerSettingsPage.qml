import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    property string serverUuid: ""

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: theCol.height
        DialogHeader
        {}
        Column {
            id:theCol
            spacing:10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:parent.top

            PageHeader {
                title: qsTr("Server Settings")
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
            }

            TextField {
                id: serverLogin
                text: appSettings.server(serverUuid).login
                anchors.left: parent.left;
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                inputMethodHints: Qt.ImhNoAutoUppercase
                label:qsTr("Login");
                placeholderText: qsTr("Login");
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                text: qsTr("Password used to authenticate to kodi. Leave blank if no password is used. Note that it \
will be stored unencrypted on the device")
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
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                text: qsTr("TCP port used to connect to the server. Unless you changed it in kodi, \
the default value should be fine. You need to enable remote access in kodi, otherwise notifications \
will not function properly.")
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
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.WordWrap
                color: Theme.highlightColor
                text: qsTr("HTTP port used to connect to the server. Unless you changed it in kodi, \
the default value (8080) should be fine. \
You need to enable HTTP remote access in kodi for this to work")
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
        if(serverVolumePlugin.currentIndex === 0)
            appSettings.server(serverUuid).setKodiVolumePlugin();
        else
            appSettings.server(serverUuid).setMinidspVolumePlugin(miniDSPAddress.text);
        appSettings.save();
    }
}
