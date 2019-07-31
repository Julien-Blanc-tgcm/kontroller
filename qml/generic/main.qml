import QtQuick 2.9
import QtQuick.Controls 2.2
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
    ThemeInformation {
        id:theme
    }

    Rectangle
    {
        anchors.fill: parent

        Rectangle {
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right: parent.right
            height:50
            color:"white"
            MouseArea {
                anchors.fill: parent
                onClicked: pageStack.pop()
            }
            Text {
                text: pageStack.depth
                color:"black"
            }

            id:topbar
        }
        StackView
        {
            anchors.top: topbar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            id:pageStack
//            anchors.fill: parent
            Keys.onPressed: {
                if(event.Key === Qt.Key_Escape)
                {
                    console.log("Popping current page")
                    pageStack.pop()
                }
            }
            initialItem: KontrollerMain
            {
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
    }


    StatusService {
        id:status
        onConnectionStatusChanged: {
            if(connectionStatus === 2)
            {
                // main.activateCurrentlyPlayingPage()
            }
        }
    }

    Component.onCompleted: {
        theme.configure(deviceInformation)
        if(!status.settingsSet)
        {
            main.activateSettingsPage()
        }
    }
}
