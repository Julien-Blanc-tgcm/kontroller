import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0

CoverBackground {
    anchors.fill: parent

    property var player: appClient.playerService.players.length > 0? appClient.playerService.players[0]:null

    Label {
        id:lbl
        visible: player && player.playingInformation
        text:player && player.playingInformation?
                     player.playingInformation.currentItem.label:""
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        wrapMode: Text.WordWrap
        font.pixelSize: Theme.fontSizeExtraSmall
        horizontalAlignment: Text.AlignHCenter
    }
    Label {
        visible: !player || !player.playingInformation
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        wrapMode: Text.Wrap
        font.pixelSize: Theme.fontSizeExtraLarge
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Nothing playing")
    }
    Image {
        source: player?player.playingInformation.currentItem.thumbnail:""
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
        enabled: player && player.playingInformation
        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: {
                if(player)
                {
                    player.next();
                }
            }
        }
        CoverAction {
            iconSource: {
                if(player && player.speed > 0)
                    return "image://theme/icon-cover-pause"
                return "image://theme/icon-cover-play"
            }
            onTriggered: {
                if(player)
                {
                    player.playPause();
                }
            }
        }
    }

}
