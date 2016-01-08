import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."

Page {
    Remote {
        id: remoteController
    }
    property int buttonWidth : 150;

    SilicaFlickable {
        anchors.fill: parent
        anchors.topMargin:2 * Theme.itemSizeSmall
        IconButton {
            width: buttonWidth
            height: buttonWidth
            id: btnUp
            anchors.bottom: btnOk.top
            anchors.bottomMargin: 5
            anchors.left: btnOk.left
            onClicked: remoteController.up()
            icon.source:"image://theme/icon-m-up"
            icon.scale: 2
        }
        IconButton {
            id:btnDown
            width: buttonWidth
            height: buttonWidth
            anchors.top: btnOk.bottom
            anchors.topMargin: 5
            anchors.left: btnOk.left
            icon.source: "image://theme/icon-m-down"
            icon.scale: 2
            onClicked: remoteController.down()
        }
        IconButton {
            id:btnLeft
            width: buttonWidth
            height: buttonWidth
            anchors.bottom: btnOk.bottom
            anchors.right: btnOk.left
            anchors.rightMargin: 5
            icon.source: "image://theme/icon-m-left"
            icon.scale: 2
            onClicked: remoteController.left()
        }
        IconButton {
            id: btnRight
            width: buttonWidth
            height: buttonWidth
            anchors.bottom: btnOk.bottom
            anchors.left: btnOk.right
            anchors.leftMargin: 5
            icon.source: "image://theme/icon-m-right"
            icon.scale: 2
            onClicked: remoteController.right()
        }
        IconButton {
            width: buttonWidth
            height: buttonWidth
            anchors.top: parent.top
            anchors.topMargin : 5
            anchors.left: parent.left
            anchors.leftMargin: 5
            icon.source: "image://theme/icon-m-back"
            icon.scale: 2
            onClicked: remoteController.back()
        }
        IconButton {
            width: buttonWidth
            height: buttonWidth
            anchors.top:parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            icon.source: "image://theme/icon-m-home"
            icon.scale: 2
            onClicked: remoteController.home()
        }
        IconButton {
            width: buttonWidth
            height: buttonWidth
            anchors.bottom: btnUp.bottom
            anchors.left: btnLeft.left
            icon.source: "image://theme/icon-m-question"
            icon.scale: 2
            onClicked:remoteController.info()
        }
        IconButton{
            width: buttonWidth
            height: buttonWidth
            anchors.bottom: btnUp.bottom
            anchors.left: btnRight.left
            icon.source: "image://theme/icon-m-menu"
            icon.scale: 2
            onClicked:remoteController.contextMenu()
        }
        IconButton {
            width: buttonWidth
            height: buttonWidth
            id: btnOk
            anchors.horizontalCenter: parent.horizontalCenter
            y: buttonWidth * 2 + 10
            icon.scale: 2
            icon.source: "image://theme/icon-m-acknowledge"
            onClicked: remoteController.select()
        }
        IconButton {
            width: buttonWidth
            height:buttonWidth
            id:btnOsd
            anchors.bottom:btnDown.bottom
            anchors.right: btnRight.right
            icon.scale: 2
            icon.source: "image://theme/icon-m-acknowledge"
            onClicked: remoteController.showOSD();
        }
    }
}
