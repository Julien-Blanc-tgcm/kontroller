import QtQuick 2.6
import Sailfish.Silica 1.0

CoverBackground {
    Label {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.paddingSmall
        anchors.rightMargin: Theme.paddingSmall
        wrapMode: Text.Wrap
        font.pixelSize: Theme.fontSizeExtraLarge
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Not connected to the server")
    }
    IconButton {
        id:unconnected
        anchors.top:parent.top
        icon.source: "image://theme/icon-s-warning"
    }

/*    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-pause"
        }
    } */
}
