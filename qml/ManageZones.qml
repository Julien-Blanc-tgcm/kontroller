import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0
import "."


Page {

    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: Theme.horizontalPageMargin
        spacing:Theme.paddingSmall

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.rightMargin: Theme.horizontalPageMargin
            text:qsTr("To learn new zones, start polling and then switch the audio output inside Kodi. Zones will be added automatically. Don’t forget go back and save afterwhile")
            wrapMode: Text.WordWrap
            color:Theme.highlightColor
        }

        Repeater {
            model:settings.servers[settings.currentServerIdx].zones
            Label {
                text:modelData
            }
        }
    }

    Component.onCompleted: {
        //settings.pollForZones();
    }
    Component.onDestruction: {
        settings.endPolling();
    }
}
