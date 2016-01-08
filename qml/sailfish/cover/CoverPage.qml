import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0

CoverBackground {
    anchors.fill: parent

    PlayingInformation {
        id:playingInformation
    }
    Label {
        id:lbl
        text:playingInformation.currentItem?playingInformation.currentItem.label:""
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        wrapMode: Text.WordWrap
        font.pixelSize: Theme.fontSizeExtraSmall
        horizontalAlignment: Text.AlignHCenter
    }
    Image {
        source: playingInformation.currentItem?playingInformation.currentItem.thumbnail:""
        anchors.top:lbl.bottom
        anchors.left: parent.left
        anchors.leftMargin: Theme.paddingSmall
        anchors.rightMargin: Theme.paddingSmall
        anchors.right: parent.right
        height:parent.height - coverAction.height - lbl.height
        fillMode: Image.PreserveAspectFit
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: console.log("Next")
        }
        CoverAction {
            iconSource: "image://theme/icon-cover-pause"
            onTriggered: console.log("Pause");
        }
    }
}
