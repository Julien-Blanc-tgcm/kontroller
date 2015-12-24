import QtQuick 2.0

import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0
import "."

Page {
    id:page
    Settings {
        id: settings
    }


    function getLeftMargin()
    {
        var x = Math.max(serverText.width, serverPortText.width) + 10
        if(x < parent.width / 4)
            return x + 30;
        else
            return x + 5;
    }

    SilicaFlickable {
        anchors.fill: parent
        Column {
            spacing:10
            anchors.fill: parent
            PageHeader {
                title: qsTr("Settings")
            }
            TextField {
                id: serverAddress
                text:settings.serverAddress
                anchors.left: parent.left;
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
                label:qsTr("Server address");
            }

            TextField {
                id: serverPort
                text:settings.serverPort
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                validator: IntValidator { bottom: 1; top: 65535 }
                inputMethodHints: Qt.ImhDigitsOnly
                label:qsTr("Server port")
            }
            TextSwitch {
                id:chkIgnoreWifi
                checked: settings.ignoreWifiStatus
                text:qsTr("Ignore wi-fi status")
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.right
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
                    settings.setServer(serverAddress.text, serverPort.text)
                    deviceInformation.update()
                }
            }
        }
    }

    Binding {
        target: settings
        property: "serverAddress"
        value: serverAddress.text
    }
    Binding {
        target:settings
        property: "serverPort"
        value:serverPort.text
    }
/*    Binding{
        target:settings
        property: "videosFileBrowsing"
        value: chkVideosFileBrowsing.checked
    }
    Binding {
        target: settings
        property: "musicFileBrowsing"
        value: chkMusicFileBrowsing.checked;
    }
    Binding {
        target: settings
        property: "useHttpInterface"
        value: chkUseHttpInterface.checked
    } */
    Binding {
        target:settings
        property: "ignoreWifiStatus"
        value:chkIgnoreWifi.checked
    }
}
