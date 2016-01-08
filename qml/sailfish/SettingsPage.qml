import QtQuick 2.0

import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0
import "."

Page {
    id:page
    Settings {
        id: settings
    }

    SilicaFlickable {
        anchors.fill: parent
        PullDownMenu {
            MenuItem {
                text:qsTr("New server")
                onClicked: settings.newServer("Test")
            }
            MenuItem {
                text:qsTr("Delete server")
                onClicked:settings.deleteCurrentServer()
                visible: settings.servers.length > 1
            }
            Repeater {
                model:settings.servers
                delegate: MenuItem {
                    text:modelData.name
                    onClicked:settings.setCurrentServerIdx(index)
                }
            }
        }
        Column {
            spacing:10
            anchors.fill: parent
            PageHeader {
                title: qsTr("Settings")
            }
            TextField {
                id:serverName
                text:settings.servers[settings.currentServerIdx].name
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                label:qsTr("Server name");
                placeholderText: qsTr("Server name")
            }

            TextField {
                id: serverAddress
                text:settings.servers[settings.currentServerIdx].serverAddress
                anchors.left: parent.left;
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
                label:qsTr("Server address");
                placeholderText: qsTr("Server address");
            }

            TextField {
                id: serverPort
                text:settings.servers[settings.currentServerIdx].serverPort
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                validator: IntValidator { bottom: 1; top: 65535 }
                inputMethodHints: Qt.ImhDigitsOnly
                label:qsTr("Server port")
                placeholderText: qsTr("Server port")
            }
            TextField {
                id : serverHttpPort
                text:settings.servers[settings.currentServerIdx].serverHttpPort
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                validator: IntValidator{ bottom:1; top:65535}
                inputMethodHints: Qt.ImhDigitsOnly
                label:qsTr("Web port")
                placeholderText: qsTr("Web port")
            }

            TextSwitch {
                id:chkIgnoreWifi
                checked: settings.ignoreWifiStatus
                text:qsTr("Ignore wi-fi status")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
            }

/*        TextSwitch {
            id: chkUseHttpInterface
            checked:settings.useHttpInterface
            text: qsTr("Use HTTP transport")
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.top: serverPort.bottom
            anchors.topMargin:10
        }

        TextSwitch {
            id : chkMusicFileBrowsing
            checked: settings.musicFileBrowsing
            text: qsTr("Use file browsing for music")
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.top: chkUseHttpInterface.bottom
            anchors.topMargin:10
        }
        TextSwitch {
            id: chkVideosFileBrowsing
            checked:settings.videosFileBrowsing
            text: qsTr("Use file browsing for videos")
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.top: chkMusicFileBrowsing.bottom
            anchors.topMargin:10
        } */
/*    Text {
        id:labelDpi
        text: qsTr("Dpi")
        anchors.left: parent.left
        anchors.leftMargin:  5
        anchors.verticalCenter:valueDpi.verticalCenter
        font.bold: true
    }

    TextField {
        id: valueDpi
        text:settings.dpi
        anchors.left: parent.left
        anchors.leftMargin: getLeftMargin()
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: chkVideosFileBrowsing.bottom
        anchors.topMargin:10
        validator: IntValidator { bottom: 25; top: 65535 }
        inputMethodHints: Qt.ImhDigitsOnly
    } */
            Button {
                text: "Save"
                anchors.right: parent.right
                anchors.rightMargin: 10
                onClicked: {
                    // settings.musicFileBrowsing = chkMusicFileBrowsing.checked;
                    //settings.videosFileBrowsing = chkVideosFileBrowsing.checked;
                    settings.servers[settings.currentServerIdx].setServerAddress(serverAddress.text);
                    settings.servers[settings.currentServerIdx].setServerPort(serverPort.text);
                    settings.servers[settings.currentServerIdx].setServerHttpPort(serverHttpPort.text);
                    settings.servers[settings.currentServerIdx].setName(serverName.text)
                    settings.setIgnoreWifiStatus(chkIgnoreWifi.checked);
                    settings.save();
                    deviceInformation.update()
                }
            }
        }
    }
}
