import QtQuick 2.6
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."
import "utils.js" as Utils

Page {
    id: item1

    property var client: appClient
    property var player: appClient.playerService.players.length > 0 ? appClient.playerService.players[0] : null

    property bool hidePanel: true // on this page, hide the docked panel: it is redundant

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: theCol.height

        PullDownMenu {
            MenuItem {
                text: qsTr("Remote control")
                onClicked: pushRemotePage()
            }
        }

        Column
        {
            id: theCol
            width:parent.width
            PageHeader {
                title: qsTr("Currently playing")
                id:theHeader
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                height: thumbnailImg.height + Theme.paddingMedium * 2
                color:"transparent"
                visible: somethingPlaying()
                Image {
                    anchors.top:parent.top
                    anchors.right: parent.right
                    anchors.topMargin: Theme.paddingMedium
                    anchors.rightMargin: Theme.paddingMedium
                    source: player?player.playingInformation.currentItem.thumbnail:""
                    height:Theme.iconSizeExtraLarge
                    width:Theme.iconSizeExtraLarge
                    id:thumbnailImg
                    fillMode: Image.PreserveAspectFit
                    z: 2
                    MouseArea {
                        anchors.fill: parent
                        onClicked: handleClickOnThumbDisplay();
                    }
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
                Column {
                    anchors.left: parent.left
                    anchors.right: thumbnailImg.left
                    anchors.leftMargin: Theme.horizontalPageMargin
                    anchors.rightMargin: Theme.horizontalPageMargin
                    anchors.verticalCenter: thumbnailImg.verticalCenter
                    spacing: Theme.paddingSmall
                    Label {
                        text: player?getMainDisplay(player.playingInformation.currentItem):"";
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        id:mainDisplay
                        wrapMode: Text.Wrap
                        MouseArea {
                            anchors.fill: parent
                            onClicked: handleClickOnMainDisplay()
                        }
                    }
                    Label {
                        text:player?getSubDisplay(player.playingInformation.currentItem):""
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        color: Theme.highlightColor
                        font.italic: true
                        id:subDisplay
                        wrapMode: Text.WordWrap
                    }
                }

            }

            Label {
                text: qsTr("Nothing playing")
                visible: !somethingPlaying()
                color: Theme.highlightColor
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                wrapMode: Text.Wrap
            }

            PlayerControl {
                y: Math.max(thumbnailImg.y + thumbnailImg.height, subDisplay.y + subDisplay.height) + 10
                anchors.left: parent.left
                anchors.right: parent.right
                player: item1.player
                id:playerControl
                visible: somethingPlaying()
                showLabel: false
            }

            SectionHeader {
                text: qsTr("Playing options")
                visible: somethingPlaying()
            }

            PlayerProperties {
                player: item1.player;
                visible: somethingPlaying()
                anchors.left: parent.left
                anchors.right: parent.right
            }

            SectionHeader {
                text: qsTr("Volume")
            }
            VolumeControl {
                anchors.left: parent.left
                anchors.right: parent.right
                volumePlugin: appClient?appClient.volumePlugin:null
                id: volumeControl
            }
            Image {
                anchors.left: parent.left
                anchors.right: parent.right
                source : player?player.playingInformation.currentItem.fanart:""
                id:fanartImg
                fillMode: Image.PreserveAspectFit
                visible: somethingPlaying()
            }
        }
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

    function somethingPlaying()
    {
        return player && player.playingInformation.currentItem
    }

    function handleClickOnMainDisplay()
    {
        if(player) {
            var curItem = player.playingInformation.currentItem;
            if(!curItem)
                return;
            if(curItem.type === "song")
            { // main display is the artist
                mediaInformationClicked({"filetype": "artist", "id": curItem.artistId, "label": curItem.label}); // go to the artist page
            }
            else if(curItem.type === "movie")
            {
                mediaInformationClicked({"filetype": "movie", "id": curItem.movieId, "label": curItem.label}); // go to the movie page
            }
            else if(curItem.type === "episode")
            {
                mediaInformationClicked({"filetype": "tvshow", "id": curItem.tvshowId, "label": curItem.label}); // go to the tvshow page
            }
        }
    }

    function handleClickOnThumbDisplay()
    {
        if(player) {
            var curItem = player.playingInformation.currentItem;
            if(!curItem)
                return;
            if(curItem.type === "song")
            { // thumb display is the album
                mediaInformationClicked({"filetype": "album", "id": curItem.albumId, "label": curItem.label}); // go to the artist page
            }
            else if(curItem.type === "movie")
            {
                mediaInformationClicked({"filetype": "movie", "id": curItem.movieId, "label": curItem.label}); // go to the movie page
            }
            else if(curItem.type === "episode")
            {
                mediaInformationClicked({"filetype": "episode", "id": curItem.episodeId, "label": curItem.label}); // go to the episode
            }
        }
    }

    signal mediaInformationClicked(var file)
}

