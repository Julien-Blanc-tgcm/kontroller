import QtQuick 2.2
import QtQuick.Controls 1.1
import eu.tgcm 1.0

ApplicationWindow {
    visible: true
    width: 480
    height: 800
    title: qsTr("Kodi Remote")
   // color: "#000"

    property alias scalingFactor : deviceInformation.scalingFactor
    property alias touchScalingFactor: deviceInformation.touchScalingFactor

    DeviceInformation {
        id:deviceInformation
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Remote")
                onTriggered: main.activateRemotePage()
            }
            MenuItem {
                text: qsTr("Music")
                onTriggered: main.activateMusicPage()
            }
            MenuItem {
                text: qsTr("Videos")
                onTriggered: main.activateVideosPage()
            }

            MenuItem {
                text: qsTr("Settings")
                onTriggered: main.activateSettingsPage()
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    KodiMain {
        id: main
        Keys.onReleased: {
            if(event.key === Qt.Key_Back || event.key === Qt.Key_Backspace)
            {
                console.log(event.key)
                event.accepted = true;
            }
        }
        anchors.fill: parent
    }
}
