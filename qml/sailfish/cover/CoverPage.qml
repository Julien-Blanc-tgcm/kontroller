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
            onTriggered: {
                if(playerController.players.length > 0)
                {
                    var player = playerController.players[0];
                    playerController.next(player.playerId);
                }
            }
        }
        CoverAction {
            iconSource: {
                if(playerController.players.length > 0)
                {
                    var player = playerController.players[0];
                    if(player.speed > 0)
                        return "image://theme/icon-cover-pause"
                }
                return "image://theme/icon-cover-play"
            }
            onTriggered: {
                if(playerController.players.length > 0)
                {
                    var player = playerController.players[0];
                    playerController.playPause(player.playerId);
                }
            }
        }
    }

    PlayerControl {
        id:playerController
    }
}
