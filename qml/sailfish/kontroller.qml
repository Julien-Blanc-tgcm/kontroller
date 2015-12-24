import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0

ApplicationWindow {

   // color: "#000"

//    property alias scalingFactor : deviceInformation.scalingFactor
//    property alias touchScalingFactor: deviceInformation.touchScalingFactor

    allowedOrientations: Orientation.Portrait
    _defaultPageOrientations: Orientation.All
    cover:Qt.resolvedUrl("cover/CoverPage.qml")
    initialPage: Component {
        KontrollerMain {
            anchors.fill: parent
        }

/*            Component.onCompleted: {
                if(!status.settingsSet)
                {
                    pageStack.push(Qt.resolvedUrl("SettingsPage.qml"));
                }
            }*/
    }
    StatusService {
        id:status
        onConnectionStatusChanged: {
            if(connectionStatus === 2)
            {
                pageStack.push(Qt.resolvedUrl("CurrentlyPlaying.qml"))
            }
        }
    }
    DeviceInformation {
        id:deviceInformation
    }

  /*  menuBar: MenuBar {
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
                text: qsTr("Now playing")
                onTriggered: main.activateCurrentlyPlayingPage()
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
    } */

}
