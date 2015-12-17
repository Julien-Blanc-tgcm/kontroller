import QtQuick 2.0
import "."
Item {
    property var information

    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100
        anchors.left: parent.left
        anchors.right: parent.right
        color:Styling.textColor
        text: qsTr("Currently playing:") + " " + information.mediaTitle
    }
    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 80
        anchors.left: parent.left
        anchors.right: parent.right
        color:Styling.textColor
        text: qsTr("Album:") + " " + (information.currentItem?
                                          information.currentItem.album:"???")
    }
    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 60
        anchors.left: parent.left
        anchors.right: parent.right
        color:Styling.textColor
        text: qsTr("Artist:") + " " + (information.currentItem?
                  information.currentItem.artist:"???")
    }

}

