import QtQuick 2.0
import eu.tgcm 1.0
import QtQuick.Controls 1.4
import "."
import "utils.js" as Utils

Item {
    id: item1

    KodiPlayingInformation {
        id:playingInformation
    }

    PlayerControl {
        id:playerController
    }

    property int activePlayer : 0

    Image {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        source : playingInformation.currentItem?playingInformation.currentItem.fanart:""
        id:fanartImg
        height: {
            if(sourceSize.height != 0)
                return width * sourceSize.height / sourceSize.width
            else
                return 200
        }
    }

    Image {
        anchors.top:parent.top
        anchors.right: parent.right
        anchors.topMargin: 10 * scalingFactor
        anchors.rightMargin: 10 * scalingFactor
        source: playingInformation.currentItem?playingInformation.currentItem.thumbnail:""
        height:width
        width:parent.width / 3
        id:thumbnailImg
        fillMode: Image.PreserveAspectFit
    }
    Rectangle {
        anchors.fill:thumbnailImg
        color:Styling.textColor
        visible : thumbnailImg.sourceSize.width === 0
        Rectangle {
            anchors.topMargin : 1 * scalingFactor
            anchors.bottomMargin: 1 * scalingFactor
            anchors.leftMargin: 1 * scalingFactor
            anchors.rightMargin: 1 * scalingFactor
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color:"#000"
            Text {
                anchors.fill: parent
                color:Styling.textColor
                font.pixelSize: 12 * scalingFactor
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:internal.getThumbDisplay(playingInformation.currentItem);
            }
        }
    }



    Text {
        anchors.left: parent.left
        anchors.leftMargin: 10 * scalingFactor
        anchors.top:parent.top
        anchors.topMargin: 10 * scalingFactor
        anchors.right: thumbnailImg.left
        anchors.rightMargin: 10 * scalingFactor
        text: internal.getMainDisplay(playingInformation.currentItem);
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 16 * scalingFactor
        color:Styling.textColor
        id:mainDisplay
        wrapMode: Text.WordWrap
    }
    Text {
        anchors.left: parent.left
        anchors.leftMargin: 10 * scalingFactor
        anchors.right: thumbnailImg.left
        anchors.rightMargin: 10 * scalingFactor
        anchors.top: mainDisplay.bottom
        text:internal.getSubDisplay(playingInformation.currentItem)
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 14 * scalingFactor
        color:Styling.textColor
        font.italic: true
        id:subDisplay
        wrapMode: Text.WordWrap
    }

    Slider {
        id: progressSlider
        anchors.left: parent.left
        anchors.right: parent.right
        //anchors.top:subDisplay.bottom
        y: Math.max(thumbnailImg.y + thumbnailImg.height, subDisplay.y + subDisplay.height) + 10 * scalingFactor
        anchors.leftMargin: 10 * scalingFactor
        anchors.rightMargin: 10 * scalingFactor
        minimumValue: 0
        maximumValue: 100
        value: internal.getPercentage(activePlayer)
        onPressedChanged: {
            if(!pressed && value !== internal.getPercentage(activePlayer) && internal.canSeek(activePlayer))
                internal.seekTo(value)
        }
    }

    Text {
        anchors.left: progressSlider.left
        anchors.top:progressSlider.bottom
        text:internal.getTime(activePlayer)
        color:Styling.textColor
        font.pixelSize: 10 * scalingFactor
        id:lblCurrentTime
    }
    Text {
        anchors.right: progressSlider.right
        anchors.top:progressSlider.bottom
        text:internal.getTotalTime(activePlayer)
        color:Styling.textColor
        font.pixelSize: 10 * scalingFactor
        id:lblTotalTime
    }

    Text {
        anchors.left:parent.left
        anchors.right: parent.right
        anchors.rightMargin: lblTotalTime.width + progressSlider.anchors.leftMargin * 2
        anchors.leftMargin: lblTotalTime.width + progressSlider.anchors.rightMargin * 2
        anchors.top:progressSlider.bottom
        font.pixelSize: 10 * scalingFactor
        horizontalAlignment: Text.AlignHCenter
        color:Styling.textColor
        text: internal.getNextDisplay(playingInformation.nextItem)
        elide: Text.ElideMiddle
        clip:true
    }


    Repeater {
        id:rptr
        model: [
            {"icon":"stop.png", "action":"stop"},
            {"icon":"prev.png", "action":"prev"},
            {"icon":"backward.png", "action":"backward"},
            {"icon":"play.png", "action":"playpause"},
            {"icon":"forward.png", "action":"forward"},
            {"icon":"next.png", "action":"next"}
        ]
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: lblCurrentTime.bottom
        height:15 * touchScalingFactor
        delegate: Image {
            x: {
                var itemWidth = parent.width / rptr.model.length;
                var marginLeft = (itemWidth - rptr.height) / 2
                return index * itemWidth + marginLeft;
            }
            y: rptr.y
            width:rptr.height
            height:rptr.height
            source:"icons/" + model.modelData.icon
            MouseArea {
                anchors.fill: parent
                onClicked: internal.executeCommand(model.modelData.action);
            }
        }
    }

    PlayerProperties {
        player: visible?playerController.players[activePlayer]:null;
        visible:activePlayer < playerController.players.length
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top:rptr.bottom
        height: 15 * touchScalingFactor
    }

    QtObject {
        id:internal
        function getMainDisplay(item) {
            if(!item)
                return qsTr("Nothing playing")
            if(item.type === "song")
            {
                var ret = item.artist;
                if(!ret)
                    return qsTr("Unknow artist");
                return ret;
            }
            else if(item.type === "movie")
                return item.label;
            else if(item.type === "episode")
                return item.tvshow;
            else
                return item.label
        }

        function getSubDisplay(item) {
            if(!item)
                return "";
            if(item.type === "song")
            {
                var ret = item.label;
                if(!ret)
                    return item.file;
                return ret
            }
            else if(item.type === "movie")
                return "Movie";
            else if(item.type === "episode")
                return item.label;
            else
                return "";
        }

        function getThumbDisplay(item)
        {
            if(!item)
                return "";
            if(item.type === "song")
            {
                return item.album;
            }
            else
                return "";
        }

        function getPlayer(playerIndex)
        {
            if(playerIndex < playerController.players.length)
                return playerController.players[playerIndex];
            return null;
        }

        function getTime(player)
        {
            var activePlayer = getPlayer(player);
            if(activePlayer)
                return Utils.formatMsecTime(activePlayer.time);
            return "-";
        }

        function getTotalTime(player)
        {
            var activePlayer = getPlayer(player);
            if(activePlayer)
                return Utils.formatMsecTime(activePlayer.totalTime);
            return "-";
        }

        function getPercentage(player)
        {
            var activePlayer = getPlayer(player)
            if(activePlayer)
                return activePlayer.percentage;
            else
                return 0;
        }

        function activePlayerId()
        {
            if(activePlayer < playerController.players.length)
            {
                return playerController.players[activePlayer].playerId;
            }
            return -1;
        }

        function executeCommand(command)
        {
            var playerId = activePlayerId();
            if(playerId === -1)
                return;
            if(command === "stop")
                playerController.stop(playerId);
            if(command === "prev")
                playerController.previous(playerId);
            if(command === "playpause")
                playerController.playPause(playerId);
            if(command === "next")
                playerController.next(playerId);
            if(command === "stop")
                playerController.stop(playerId);
            if(command === "backward")
                playerController.seekBackward(playerId);
            if(command === "forward")
                playerController.seekForward(playerId);
        }

        function getNextDisplay(item)
        {
            if(!item)
                return ""
            else if(item.type === "song")
                return qsTr("Next: <i>%1</i> by <i>%2</i>").arg(item.label).arg(item.artist)
            else if(item.type === "unknown")
                return qsTr("Next: <i>%1</i>").arg(item.label)
            else
                return item.type;
        }

        function seekTo(percentage)
        {
            var playerId = activePlayerId();
            if(playerId === -1)
                return;
            else
                playerController.seekToPercentage(playerId, percentage)
        }

        function canSeek(player)
        {
            if(player >= playerController.players.length)
                return;
            else
                return playerController.players[player].canSeek;
        }
    }
}

