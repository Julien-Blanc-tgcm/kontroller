import QtQuick 2.6
import Sailfish.Silica 1.0

CoverBackground {
    Label {
        id: label
        anchors.centerIn: parent
        text: qsTr("Kontroller")
    }
    IconButton {
        id:unconnected
        anchors.top:label.bottom
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
