import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "utils.js" as Utils
import "."

Page {
    id:main
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: theCol.childrenRect.height + header.height
        VerticalScrollDecorator {}

        PullDownMenu {
            MenuItem {
                text:qsTr("Remote control")
                onClicked: remoteClicked()
            }
            MenuItem {
                text:qsTr("Currently playing")
                onClicked: currentClicked()
            }
            MenuItem {
                text:qsTr("Back to menu")
                onClicked: backToMenuClicked();
            }
        }

        PageHeader {
            id:header
            title:service.showTitle
        }
        Image {
            id: fanart
            source: service.thumbnail //(service.thumbnail.length > 0)?("image://kodi/" + service.thumbnail):""
            height:parent.width / 3
            width:parent.width / 3
            fillMode: Image.PreserveAspectFit
            anchors.right: parent.right
            anchors.top:header.bottom
        }
        Column {
            id:theCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:header.bottom
            Repeater {
                id:properties
                model:[
                    {text:qsTr("Episodes:"), value:service.nbEpisodes},
                    {text:qsTr("Watched:"), value:service.nbWatchedEpisodes}
                ]
            delegate: Item {
                    height:Theme.itemSizeSmall
                    anchors.left: theCol.left
                    anchors.right: theCol.right
                    anchors.leftMargin: Theme.horizontalPageMargin
                    Label {
                        id:txt
                        text: model.modelData.text
                        x:0
                        verticalAlignment: Text.AlignVCenter
                        color:Theme.highlightColor
                        font.bold: true
                    }
                    Label {
                        anchors.left: txt.right
                        anchors.leftMargin: Theme.paddingSmall
                        text:typeof(model.modelData.value) !== "undefined"?model.modelData.value:""
                        color:Theme.highlightColor
                    }
                }
            }

            Label {
                id:episodesLbl
                anchors.left: parent.left
                anchors.right: parent.right
                text:qsTr("Episodes:")
                color:Theme.highlightColor
                font.bold: true
                anchors.leftMargin: Theme.horizontalPageMargin
            }

            Repeater {
                id:songs
                model:service.episodes
                delegate: Item {
                    height:Theme.itemSizeSmall
                    width:theCol.width
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: Theme.horizontalPageMargin
                    anchors.rightMargin: Theme.horizontalPageMargin
                    IconButton {
                        icon.source: "image://theme/icon-m-play"
                        width:height
                        height:parent.height
                        anchors.left: parent.left
                        onClicked: control.playFile(model.modelData)
                    }

                    Label {
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        x: height + Theme.paddingSmall
                        text:model.modelData.label
                        clip:true
                        elide: Text.ElideMiddle
                        width:parent.width - height - Theme.paddingSmall
                        MouseArea {
                            anchors.fill: parent
                            onClicked: mediaInformationClicked(model.modelData.filetype,
                                                                   model.modelData.file,
                                                                   model.modelData.label)
                            onPressAndHold: {
                                theSubMenu.showSubMenu(parent, model.modelData, mouseX, mouseY)
                            }
                        }
                    }
                }
            }
        }
    }

/*    ListContextMenu {
        visible:false
        color:"black"
        border.color: Styling.borderColor
        border.width: 2 * scalingFactor
        id:theSubMenu
        onInformationPressed:
        {
            visible = false
            if(currentModel)
                mediaInformationClicked(currentModel.filetype, currentModel.file, currentModel.label)
        }
        onAddToPlaylistPressed:  {
            visible = false
            control.addToPlaylist(currentModel)
        }
        onPlayPressed: {
            visible = false
            control.playFile(currentModel)
        }
    } */

    SeasonInformationService {
        id:service
    }

    VideoControl {
        id:control
    }

    property alias itemId: service.seasonId
    property string label
    onItemIdChanged: { service.refresh(); }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }
}

