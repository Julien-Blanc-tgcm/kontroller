import QtQuick 2.0
import Sailfish.Silica 1.0
import "utils.js" as Utils


Item {

    property bool showSlider: true
    property var player : null
    implicitHeight: rptr.y + rptr.height - progressSlider.y

    Slider {
        id: progressSlider
        anchors.left: parent.left
        anchors.right: parent.right
        //anchors.top:subDisplay.bottom
        anchors.leftMargin: Theme.paddingMedium
        anchors.rightMargin: Theme.paddingMedium
        minimumValue: 0
        maximumValue: 100
        value: getPercentage(player)
        onPressedChanged: {
            if(!pressed && player && player.canSeek)
                player.percentage = value
        }
        visible: showSlider
    }

    Label {
        anchors.left: progressSlider.left
        anchors.bottom:progressSlider.bottom
        text: getTime(player)
        font.pixelSize: Theme.fontSizeExtraSmall
        id:lblCurrentTime
        color:Theme.highlightColor
    }
    Label {
        anchors.right: progressSlider.right
        anchors.bottom:progressSlider.bottom
        text: getTotalTime(player)
        font.pixelSize: Theme.fontSizeExtraSmall
        id:lblTotalTime
        color:Theme.highlightColor
    }

    Label {
        anchors.left:parent.left
        anchors.right: parent.right
        anchors.rightMargin: lblTotalTime.width + progressSlider.anchors.leftMargin * 2
        anchors.leftMargin: lblTotalTime.width + progressSlider.anchors.rightMargin * 2
        anchors.bottom:progressSlider.bottom
        font.pixelSize: Theme.fontSizeExtraSmall
        horizontalAlignment: Text.AlignHCenter
        text: player?getCurrentDisplay(player.playingInformation.currentItem):""
        elide: Text.ElideMiddle
        clip:true
        color:Theme.highlightColor
    }

    Repeater {
        id:rptr
        model: [
            {"icon":"image://theme/icon-m-previous", "action":"prev"},
            {"icon":"image://theme/icon-m-left", "action":"backward"},
            {"icon":"image://theme/icon-m-clear", "action":"stop"},
            {"icon":["image://theme/icon-m-play", "image://theme/icon-m-pause"], "action":"playpause"},
            {"icon":"image://theme/icon-m-right", "action":"forward"},
            {"icon":"image://theme/icon-m-next", "action":"next"}
        ]
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: lblCurrentTime.bottom
        anchors.topMargin: Theme.paddingMedium
        height:Theme.iconSizeMedium
        delegate: IconButton {
            x: {
                var itemWidth = parent.width / rptr.model.length;
                var marginLeft = (itemWidth - rptr.height) / 2
                return index * itemWidth + marginLeft;
            }
            y: rptr.y
            width:Theme.iconSizeMedium
            height:Theme.iconSizeMedium
            icon.source: {
                if(typeof(model.modelData.icon) === "string")
                    return model.modelData.icon;
                else
                {
                    if(player && player.speed > 0)
                        return model.modelData.icon[1];
                    else
                        return model.modelData.icon[0];
                }
            }
            onClicked: executeCommand(model.modelData.action);
        }
    }

    function getTime(player)
    {
        if(player)
            return Utils.formatMsecTime(player.time);
        return "-";
    }

    function getTotalTime(player)
    {
        if(player)
            return Utils.formatMsecTime(player.totalTime);
        return "-";
    }

    function getPercentage(player)
    {
        if(player)
            return player.percentage;
        else
            return 0;
    }

    function executeCommand(command)
    {
        if(player === null)
            return;
        if(command === "stop")
            player.stop();
        if(command === "prev")
            player.previous();
        if(command === "playpause")
            player.playPause();
        if(command === "next")
            player.next();
        if(command === "stop")
            player.stop();
        if(command === "backward")
            player.seekBackward();
        if(command === "forward")
            player.seekForward();
    }

    function getCurrentDisplay(item)
    {
        if(!item)
            return ""
        else if(item.type === "song")
            return qsTr("<i>%1</i> by <i>%2</i>").arg(item.label).arg(item.artist)
        else if(item.type === "unknown")
            return qsTr("<i>%1</i>").arg(item.label)
        else
            return item.type;
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
}
