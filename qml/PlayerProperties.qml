import QtQuick 2.0
import eu.tgcm 1.0
import "."
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    property var player

    PlayerControl {
        id:control
    }

    Text {
        id:lblSubtitles
        text:qsTr("Subtitles") + cbxSubs.currentIndex
        font.pixelSize: 12 * scalingFactor
        y:0
        visible:player && player.type === "video"
        color:Styling.textColor
        anchors.leftMargin: 5 * scalingFactor
    }

    ComboBox {
        id:cbxSubs
        model:getSubtitlesModel(player)
        textRole: 'name'
        visible: player && player.type === "video"
        y:0
        x:lblSubtitles.contentWidth + 5 * scalingFactor
        width:200
        clip:true
        onModelChanged: {
            updateComboBoxIndex();
        }
    }

    Image {
        source: (player && player.shuffled)?"icons/shuffleenabled.png":"icons/shuffledisabled.png"
        height:parent.height
        width:height
        y:0
        x :width + 10 * scalingFactor
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if(!player || !player.canShuffle)
                    return;
                control.setShuffle(player.playerId, !player.shuffled)
            }
        }
        //visible:player && player.type === "audio"
        visible:false
    }

    function getSubtitlesModel() {
        if(player)
        {
            cbxSubs.currentIndexChanged.disconnect(switchSubtitleIfNeeded)
            return player.subtitles
        }
        else
        {
            return null;
        }
    }

    function updateComboBoxIndex() {
        if(player)
        {
            cbxSubs.currentIndexChanged.disconnect(switchSubtitleIfNeeded)
            var idx = player.currentSubtitleIndex;
            for(var i = 0; i < player.subtitles.length; ++i)
            {
                if(player.subtitles[i].index === idx)
                {
                    cbxSubs.currentIndex = i;
                }
            }
            cbxSubs.currentIndexChanged.connect(switchSubtitleIfNeeded)
        }
    }

    function switchSubtitleIfNeeded()
    {
        if(player && cbxSubs.currentIndex != -1 && cbxSubs.model)
        {
            var item = cbxSubs.model[cbxSubs.currentIndex];
            if(item && item.index !== player.currentSubtitleIndex)
            {
                console.log("Switching subs !");
                control.switchSubtitle(player.playerId, item.index)
            }
        }
    }

}

