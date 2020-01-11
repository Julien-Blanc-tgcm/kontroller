import QtQuick 2.0

import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0
import "."

Page {
    id:page

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
                    onClicked: {
                        statusService.switchToServer(modelData.uuid)
                    }
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
                onTextChanged: {
                    settings.servers[settings.currentServerIdx].setName(serverName.text)
                    settings.save();
                }
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
                onTextChanged: {
                    settings.servers[settings.currentServerIdx].setServerAddress(serverAddress.text);
                    settings.save();
                }
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
                onTextChanged: {
                    settings.servers[settings.currentServerIdx].setServerPort(serverPort.text);
                    settings.save();
                }
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
                onTextChanged: {
                    settings.servers[settings.currentServerIdx].setServerHttpPort(serverHttpPort.text);
                    settings.save();
                }
            }
            TextSwitch {
                id: serverHasZones
                text:qsTr("Use multiples zones")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                checked:settings.servers[settings.currentServerIdx].hasZones
                onCheckedChanged: {
                    settings.servers[settings.currentServerIdx].setHasZones(serverHasZones.checked);
                    pushOrPullZonePage();
                    settings.save();
                }
            }

            TextSwitch {
                id:chkIgnoreWifi
                checked: settings.ignoreWifiStatus
                text:qsTr("Ignore wi-fi status")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                onTextChanged:
                {
                    settings.setIgnoreWifiStatus(chkIgnoreWifi.checked);
                    settings.save();
                }
            }

            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargi
                color:Theme.highlightColor
                text:qsTr("Zones uses different audio output. Go to next page to learn for current zones.")
                wrapMode: Text.WordWrap
            }
        }
    }

    property var zones;

    Component.onCompleted: {
        pushOrPullZonePage();
    }

    function pushOrPullZonePage()
    {
        console.log("pushhere");
        if(chkIgnoreWifi.checked)
        {
            console.log("pushAttached");
            zones = pageStack.pushAttached(Qt.resolvedUrl("ManageZones.qml"));
        }
        else if(zones)
            pageStack.popAttached(zones);
    }
}
