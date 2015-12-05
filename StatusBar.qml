import QtQuick 2.0
import eu.tgcm 1.0

Rectangle {
    color:"#000"

    StatusService {
        id: status
    }

    property int activePlayer : 0

    signal playlistClicked(string type)

    KodiPlayerControl {
        id: playerController
        Component.onCompleted: playerController.refreshPlayerInfo()
    }

    Text {
        anchors.fill: parent
        text: "Unconnected"
        color: "#ff0000";
        visible: status.connectionStatus === 0
    }
    Text {
        anchors.fill: parent
        text: "Connecting, please wait..."
        color: "#ff0000"
        visible: status.connectionStatus === 1;
    }
    Text {
        text: "No active players"
        visible: status.connectionStatus === 2 && playerController.players.length === 0
        color: "#0000cc"
        anchors.fill: parent
    }

    Rectangle {
        color:"#000"
        anchors.fill: parent
        visible: status.connectionStatus === 2 && playerController.players.length > activePlayer
        Image {
            source : {
                if(playerController.players.length > activePlayer)
                {
                    if(playerController.players[activePlayer].type === "audio")
                        return "icons/music.png";
                    else if(playerController.players[activePlayer].type === "video")
                        return "icons/video.png";
                    else
                        return "";
                }
                return "";
            }
            anchors.left: parent.left
            id:lbltype
            height:parent.height
            width:height
        }

        Image {
            source:"icons/stop.png"
            height:parent.height
            width:height
            anchors.left: lbltype.right
            id:btnstop
            MouseArea {
                anchors.fill: parent
                onClicked: playerController.stop(activePlayer)
            }
        }

        Image {
            source: "icons/prev.png"
            height:parent.height
            width:height
            anchors.left: btnstop.right
            id: btnprev
            MouseArea {
                anchors.fill: parent
                onClicked: playerController.previous(activePlayer)
            }
        }

        Image {
            source: "icons/play.png"
            height:parent.height
            width:height
            anchors.left: btnprev.right;
            id: btnplay;
            visible:playerController.players.length > activePlayer && playerController.players[activePlayer].speed === 0
            MouseArea {
                anchors.fill: parent
                onClicked: playerController.playPause(activePlayer)
            }
        }
        Image {
            source: "icons/pause.png"
            height:parent.height
            width:height
            anchors.left: btnprev.right;
            id: btnpause;
            visible:playerController.players.length > activePlayer && playerController.players[activePlayer].speed !== 0
            MouseArea {
                anchors.fill: parent
                onClicked: playerController.playPause(activePlayer)
            }
        }

        Image {
            source: "icons/next.png"
            height:parent.height
            width:height
            anchors.left: btnplay.right
            id:btnnext
            MouseArea {
                anchors.fill: parent;
                onClicked: playerController.next(activePlayer)
            }
        }

        Image {
            source: "icons/playlist.png"
            height:parent.height
            width:height
            anchors.left: btnnext.right
            id:btnplaylist
            MouseArea {
                anchors.fill: parent;
                onClicked: playlistClicked(playerController.players[activePlayer].type)
            }
        }

    }

}
