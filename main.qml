import QtQuick 2.2
import QtQuick.Controls 1.1
import eu.tgcm 1.0

ApplicationWindow {
    visible: true
    width: 480
    height: 800
    title: qsTr("Kodi Remote")

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
    }

}
