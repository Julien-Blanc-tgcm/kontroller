import QtQuick 2.0
import harbour.eu.tgcm 1.0
import "."
import Sailfish.Silica 1.0

Item {
    property var player
    implicitHeight: theCol.height
    Column {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: Theme.paddingSmall
        id:theCol
        ComboBox {
            visible: player && player.type === "video" && player.subtitles.length > 0
            id:cbxSubs
            label:qsTr("Subtitles")
            clip:true
            menu: ContextMenu {
                Repeater {
                    id:subsRptr
                    model:player?player.subtitles:[]
                    delegate: MenuItem {
                        text: model.name
                        onClicked: if(player) { player.currentSubtitleIndex = model.modelData.index; }
                    }
                }
            }
            currentIndex: -1
            value: ""
            Component.onCompleted: { console.log("height is " + height); }
        }
        Label {
            visible: player && player.type === "video" && player.subtitles.length === 0
            text: qsTr("No subtitles available")
            anchors.left: parent.left
            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.right: parent.right
            anchors.rightMargin: Theme.horizontalPageMargin
            color: Theme.highlightColor
        }

        ComboBox {
            id:cbxStreams
            label:qsTr("Audio")
            clip:true
            visible: player && player.type === "video"
            menu:ContextMenu {
                Repeater {
                    id:streamsRptr
                    model:player?player.audioStreams:[]
                    delegate: MenuItem {
                        text:model.displayName
                        onClicked: if(player) { player.currentAudioStream = model.modelData.index; }
                    }
                }
            }
            value:""
        }
        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            height:repeatSwitch.height
            Switch {
                id: repeatSwitch
                checked: false
                automaticCheck: false
                icon.source: "image://theme/icon-m-repeat"
                visible: player?player.canRepeat:false
                onClicked: player.repeat = (player.repeat + 1) % 3
            }
            Switch {
                id: shuffleSwitch
                checked: false
                automaticCheck: false
                icon.source: "image://theme/icon-m-shuffle"
                visible: player?player.canShuffle:false
                onClicked: player.shuffled = !player.shuffled
            }
        }

    }

    onPlayerChanged: {
        if(player)
        {
            connectPlayerEvents();
            updatePlayerProperties();
        }
    }

    function connectPlayerEvents()
    {
        player.onCurrentSubtitleIndexChanged.connect(setSubsIndex);
        player.onCurrentAudioStreamIndexChanged.connect(setAudioIndex);
        player.onShuffledChanged.connect(setPlayerShuffled);
        player.onRepeatChanged.connect(setPlayerRepeat);
    }

    function setPlayerShuffled()
    {
        if(player)
            shuffleSwitch.checked = player.shuffled;
        else
            shuffleSwitch.checked = false;
    }

    function setPlayerRepeat()
    {
        if(player)
        {
            repeatSwitch.checked = player.repeat > 0;
            repeatSwitch.icon.source = (player.repeat === 1) ?
                        "image://theme/icon-m-repeat-single" :
                        "image://theme/icon-m-repeat";
        }
        else
        {
            repeatSwitch.checked = false;
            repeatSwitch.icon.source = "image://theme/icon-m-repeat";
        }
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
        var i = getCurrentAudioIndex();
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

    Component.onCompleted:
    {
        if(player)
        {
            connectPlayerEvents();
        }
        updatePlayerProperties();
    }

    function updatePlayerProperties()
    {
        setSubsIndex();
        setAudioIndex();
        setPlayerShuffled();
        setPlayerRepeat();
    }

}

