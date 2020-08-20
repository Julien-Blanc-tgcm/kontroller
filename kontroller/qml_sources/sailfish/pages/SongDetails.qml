import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0

Item {
    id: settingsPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    property bool landscape: ( mainPage.orientation === Orientation.Landscape || mainPage.orientation === Orientation.LandscapeInverted )

    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            id: theHeader
            title: qsTr("Currently playing")
        }

        Image {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            source : player ? player.playingInformation.currentItem.fanart : ""
            id: fanartImg
            fillMode: Image.PreserveAspectFit
            visible: !landscape
        }

        Image {
            anchors.top:theHeader.bottom
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.rightMargin: 10
            source: player?player.playingInformation.currentItem.thumbnail:""
            height:Theme.iconSizeExtraLarge
            width:Theme.iconSizeExtraLarge
            id:thumbnailImg
            fillMode: Image.PreserveAspectFit
            z: 2
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
            text:player?getThumbDisplay(player.playingInformation.currentItem):""
            z: 1
            visible: thumbnailImg.sourceSize.width === 0
        }

        Label {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top:theHeader.bottom
            anchors.topMargin: 10
            anchors.right: thumbnailImg.left
            anchors.rightMargin: 10
            text: player?getMainDisplay(player.playingInformation.currentItem):"";
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
            text:player?getSubDisplay(player.playingInformation.currentItem):""
            horizontalAlignment: Text.AlignHCenter
            color:Theme.highlightColor
            id:subDisplay
            wrapMode: Text.WordWrap
        }

//        PlayerControl {
//            y: Math.max(thumbnailImg.y + thumbnailImg.height, subDisplay.y + subDisplay.height) + 10
//            anchors.left: parent.left
//            anchors.right: parent.right
//            player: item1.player
//            id:playerControl
//        }

//        PlayerProperties {
//            player: item1.player;
//            visible:player !== null
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.top: playerControl.bottom
//        }
    }

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
}
