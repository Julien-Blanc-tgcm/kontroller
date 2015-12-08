import QtQuick 2.0
import QtQuick.Controls 1.1
import eu.tgcm 1.0

Item {
    KodiRemote {
        id: remoteController
    }

    property int buttonWidth : 96;
    Rectangle {
        anchors.fill: parent
        color:"#000";
        Image {
            width: buttonWidth
            height: buttonWidth
            id: btnUp
            anchors.bottom: btnOk.top
            anchors.bottomMargin: 16
            anchors.left: btnOk.left
            source: "icons/go-up.png"
            MouseArea {
                anchors.fill: parent
                onClicked: remoteController.up()
            }
        }
        Image {
            width: buttonWidth
            height: buttonWidth
            anchors.top: btnOk.bottom
            anchors.topMargin: 16
            anchors.left: btnOk.left
            source: "icons/go-down.png"
            MouseArea {
                anchors.fill: parent
                onClicked: remoteController.down()
            }
        }
        Image {
            id:btnLeft
            width: buttonWidth
            height: buttonWidth
            anchors.bottom: btnOk.bottom
            anchors.right: btnOk.left
            anchors.rightMargin: 16
            source: "icons/go-left.png"
            MouseArea {
                anchors.fill: parent
                onClicked: remoteController.left()
            }
        }
        Image {
            id: btnRight
            width: buttonWidth
            height: buttonWidth
            anchors.bottom: btnOk.bottom
            anchors.left: btnOk.right
            anchors.leftMargin: 16
            source: "icons/go-right.png"
            MouseArea {
                anchors.fill: parent
                onClicked: remoteController.right()
            }
        }
        Image {
            width: buttonWidth
            height: buttonWidth
            anchors.top: parent.top
            anchors.topMargin : 16
            anchors.left: parent.left
            anchors.leftMargin: 16
            source: "icons/back.png"
            MouseArea {
                anchors.fill: parent
                onClicked: remoteController.back()
            }
        }
        Image {
            width: buttonWidth
            height: buttonWidth
            anchors.top:parent.top
            anchors.topMargin: 16
            anchors.right: parent.right
            anchors.rightMargin: 16
            source: "icons/home.png"
            MouseArea {
                anchors.fill: parent
                onClicked: remoteController.home()
            }
        }
        Image {
            width: buttonWidth
            height: buttonWidth
            anchors.bottom: btnUp.bottom
            anchors.left: btnLeft.left
            source:"icons/edit-find.png"
            MouseArea {
                anchors.fill: parent
                onClicked:remoteController.info()
            }
        }
        Image {
            width: buttonWidth
            height: buttonWidth
            anchors.bottom: btnUp.bottom
            anchors.left: btnRight.left
            source: "icons/properties.png"
            MouseArea {
                anchors.fill: parent
                onClicked:remoteController.contextMenu()
            }
        }
        Image {
            width: buttonWidth
            height: buttonWidth
            id: btnOk
            anchors.horizontalCenter: parent.horizontalCenter
            y: 250
            source: "icons/arrow.png"
            MouseArea{
                anchors.fill: parent
                onClicked: remoteController.select()
            }
        }

    }
    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        text : scalingFactor + " " + touchScalingFactor
        color: appstyle.textColor
        id : txtScale
    }

    Text {
        id:txtDpi
        anchors.bottom: txtScale.top
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        color:appstyle.textColor
        text: deviceInformation.dpi + " " + deviceInformation.size().width + "x" + deviceInformation.size().height
    }

    Text {
        anchors.bottom: txtDpi.top
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        color:appstyle.textColor
        text: deviceInformation.logicalDpi()
    }
}
