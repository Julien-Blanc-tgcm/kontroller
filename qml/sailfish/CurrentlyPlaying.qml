import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."
import "utils.js" as Utils

Page {
    id: item1

    PlayingInformation {
        id:playingInformation
    }

    PlayerControl {
        id:playerController
    }

    property int activePlayer : 0

    SilicaFlickable {
        anchors.fill: parent
        PageHeader {
            title: qsTr("Currently playing")
            id:theHeader
        }

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
            anchors.top:theHeader.bottom
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.rightMargin: 10
            source: playingInformation.currentItem?playingInformation.currentItem.thumbnail:""
            height:width
            width:parent.width / 3
            id:thumbnailImg
            fillMode: Image.PreserveAspectFit
        }
        Rectangle {
            anchors.fill:thumbnailImg
            color:"#fff"
            opacity:0.1
            visible : thumbnailImg.sourceSize.width === 0
            id:fakeThumb
        }
        Label {
            anchors.fill: fakeThumb
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text:internal.getThumbDisplay(playingInformation.currentItem);
        }



        Label {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top:theHeader.bottom
            anchors.topMargin: 10
            anchors.right: thumbnailImg.left
            anchors.rightMargin: 10
            text: internal.getMainDisplay(playingInformation.currentItem);
            horizontalAlignment: Text.AlignHCenter
            id:mainDisplay
            wrapMode: Text.WordWrap
        }
        Label {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.right: thumbnailImg.left
            anchors.rightMargin: 10
            anchors.top: mainDisplay.bottom
            text:internal.getSubDisplay(playingInformation.currentItem)
            horizontalAlignment: Text.AlignHCenter
            color:Styling.textColor
            id:subDisplay
            wrapMode: Text.WordWrap
        }

        Slider {
            id: progressSlider
            anchors.left: parent.left
            anchors.right: parent.right
            //anchors.top:subDisplay.bottom
            y: Math.max(thumbnailImg.y + thumbnailImg.height, subDisplay.y + subDisplay.height) + 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            minimumValue: 0
            maximumValue: 100
            value: internal.getPercentage(activePlayer)
            onPressedChanged: {
                if(!pressed && value !== internal.getPercentage(activePlayer) && internal.canSeek(activePlayer))
                    internal.seekTo(value)
            }
        }

        Label {
            anchors.left: progressSlider.left
            anchors.bottom:progressSlider.bottom
            text:internal.getTime(activePlayer)
            font.pixelSize: Theme.fontSizeExtraSmall
            id:lblCurrentTime
        }
        Label {
            anchors.right: progressSlider.right
            anchors.bottom:progressSlider.bottom
            text:internal.getTotalTime(activePlayer)
            font.pixelSize: Theme.fontSizeExtraSmall
            id:lblTotalTime
        }

        Label {
            anchors.left:parent.left
            anchors.right: parent.right
            anchors.rightMargin: lblTotalTime.width + progressSlider.anchors.leftMargin * 2
            anchors.leftMargin: lblTotalTime.width + progressSlider.anchors.rightMargin * 2
            anchors.bottom:progressSlider.bottom
            font.pixelSize: Theme.fontSizeExtraSmall
            horizontalAlignment: Text.AlignHCenter
            text: internal.getNextDisplay(playingInformation.nextItem)
            elide: Text.ElideMiddle
            clip:true
        }

        Repeater {
            id:rptr
            model: [
                {"icon":"image://theme/icon-m-clear", "action":"stop"},
                {"icon":"image://theme/icon-m-previous", "action":"prev"},
                {"icon":"image://theme/icon-m-left", "action":"backward"},
                {"icon":"image://theme/icon-m-play", "action":"playpause"},
                {"icon":"image://theme/icon-m-right", "action":"forward"},
                {"icon":"image://theme/icon-m-next", "action":"next"}
            ]
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: lblCurrentTime.bottom
            anchors.topMargin: 20
            height:60
            delegate: IconButton {
                x: {
                    var itemWidth = parent.width / rptr.model.length;
                    var marginLeft = (itemWidth - rptr.height) / 2
                    return index * itemWidth + marginLeft;
                }
                y: rptr.y
                width:rptr.height
                height:rptr.height
                icon.source:model.modelData.icon
                onClicked: internal.executeCommand(model.modelData.action);
            }
        }

        PlayerProperties {
            player: visible?playerController.players[activePlayer]:null;
            visible:activePlayer < playerController.players.length
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:rptr.bottom
            height: 15
        }
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

