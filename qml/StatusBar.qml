import QtQuick 2.0
import eu.tgcm 1.0
import "."

Rectangle {
    color:"#000"

    StatusService {
        id: status
    }

    property int activePlayer : 0

    signal playlistClicked(string type)

    KodiPlayerControl {
        id: playerController
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
        color: Styling.linkColor
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
            height:parent.height - 1
            width:height
            anchors.bottom: parent.bottom
        }

        Image {
            source:"icons/stop.png"
            height:parent.height - 1
            width:height
            anchors.left: lbltype.right
            anchors.bottom: parent.bottom
            id:btnstop
            MouseArea {
                anchors.fill: parent
                onClicked: playerController.stop(activePlayerId())
            }
        }

        Image {
            source: "icons/prev.png"
            height:parent.height - 1
            width:height
            anchors.left: btnstop.right
            anchors.bottom: parent.bottom
            id: btnprev
            MouseArea {
                anchors.fill: parent
                onClicked: playerController.previous(activePlayerId())
            }
        }

        Image {
            source: "icons/play.png"
            height:parent.height - 1
            width:height
            anchors.left: btnprev.right;
            anchors.bottom: parent.bottom
            id: btnplay;
            visible:playerController.players.length > activePlayer && playerController.players[activePlayer].speed === 0
            MouseArea {
                anchors.fill: parent
                onClicked: playerController.playPause(activePlayerId())
            }
        }

        Image {
            source: "icons/pause.png"
            height:parent.height - 1
            width:height
            anchors.left: btnprev.right;
            anchors.bottom: parent.bottom
            id: btnpause;
            visible:playerController.players.length > activePlayer && playerController.players[activePlayer].speed !== 0
            z:2
            MouseArea {
                anchors.fill: parent
                onClicked: playerController.playPause(activePlayerId())
            }
        }

        Image {
            source: "icons/next.png"
            height:parent.height - 1
            width:height
            anchors.left: btnplay.right
            anchors.bottom: parent.bottom
            id:btnnext
            MouseArea {
                anchors.fill: parent;
                onClicked: playerController.next(activePlayerId())
            }
        }

        Image {
            source: "icons/playlist.png"
            height:parent.height - 1
            width:height
            anchors.left: btnnext.right
            anchors.bottom: parent.bottom
            id:btnplaylist
            MouseArea {
                anchors.fill: parent;
                onClicked: playlistClicked(playerController.players[activePlayer].type)
            }
        }

    }
    Rectangle
    {
        color: "#0000ff"
        height:1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top:parent.top
    }
    Rectangle
    {
        color:"#0000ff"
        height:12
        x:0
        y:1
        width: {
            if(activePlayer < playerController.players.length)
                return playerController.players[activePlayer].percentage * parent.width / 100
            else
                return 0;
        }
    }

    function activePlayerId()
    {
        if(activePlayer < playerController.players.length)
        {
            return playerController.players[activePlayer].playerId;
        }
        return -1;
    }

}
