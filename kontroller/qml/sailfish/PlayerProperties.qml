import QtQuick 2.0
import harbour.eu.tgcm 1.0
import "."
import Sailfish.Silica 1.0

Item {
    property var player

    PlayerControl {
        client: appClient
        id:control
    }
    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        spacing: 10
        visible: player && player.type === "video"
        ComboBox {
            id:cbxSubs
            label:qsTr("Subtitles")
            clip:true
            menu: ContextMenu {
                Repeater {
                    id:subsRptr
                    model:player?player.subtitles:[]
                    delegate: MenuItem {
                        text: model.name
                        onClicked: control.switchSubtitle(player.playerId, model.modelData.index)
                    }
                }
            }
            currentIndex: -1
            value: ""
        }

        ComboBox {
            id:cbxStreams
            label:qsTr("Audio")
            clip:true
            menu:ContextMenu {
                Repeater {
                    id:streamsRptr
                    model:player?player.audioStreams:[]
                    delegate: MenuItem {
                        text:model.displayName
                        onClicked: control.switchAudioStream(player.playerId, model.index)
                    }
                }
            }
            currentIndex: -1
            value:""
        }
    }
    onPlayerChanged: {
        if(player)
        {
            player.onCurrentSubtitleIndexChanged.connect(setSubsIndex);
            player.onCurrentAudioStreamIndexChanged.connect(setAudioIndex);
        }
    }

    function setSubsIndex() {
        var idx = getCurrentSubsIndex();
        if(idx !== -1)
        {
            cbxSubs.currentIndex = idx;
            cbxSubs.value = player.subtitles[idx].name;
        }
        for(var i = 0; i < subsRptr.children.length; ++i)
        {
           subsRptr.children[i].down = i === idx;
        }
    }

    function getCurrentAudioIndex()
    {
        if(player)
        {
            var idx = player.currentAudioStreamIndex;
            for(var i = 0; i < player.audioStreams.length; ++i)
            {
                if(player.audioStreams[i].index === idx)
                    return i;
            }
        }
        return -1;
    }

    function setAudioIndex()
    {
        var i = this.getCurrentAudioIndex();
        if(i !== -1)
        {
            cbxStreams.currentIndex = i;
            cbxStreams.value = player.audioStreams[i].displayName;
        }
    }

    IconTextSwitch {
        icon.source:"image://theme/icon-m-shuffle"
        height:parent.height
        width:height
        y:0
        x :width + 10
        onClicked: {
            if(!player || !player.canShuffle)
                return;
            control.setShuffle(player.playerId, !player.shuffled)
        }
        //visible:player && player.type === "audio"
        visible:false
        checked: player && player.shuffled
    }

    function getCurrentSubsIndex() {
        if(player)
        {
            var idx = player.currentSubtitleIndex;
            for(var i = 0; i < player.subtitles.length; ++i)
            {
                if(player.subtitles[i].index === idx)
                    return i;
            }
        }
        return -1;
    }

    function switchSubtitleIfNeeded()
    {
        if(player && cbxSubs.currentIndex != -1)
        {
            var item = player.subtitles[cbxSubs.currentIndex];
            if(item && item.index !== player.currentSubtitleIndex)
            {
                control.switchSubtitle(player.playerId, item.index)
            }
        }
    }

    function switchAudioStreamIfNeeded()
    {
        if(player && cbxStreams.currentIndex != -1)
        {
            var item = player.audioStreams[cbxStreams.currentIndex];
            if(item && item.index !== player.currentAudioStreamIndex)
            {
                control.switchAudioStream(player.playerId, item.index)
            }
        }
    }

}

