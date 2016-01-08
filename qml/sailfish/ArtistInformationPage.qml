import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."
Page {
    id:main
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    SilicaFlickable {
        anchors.fill:parent
        contentHeight:theCol.childrenRect.height

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

        Column {
            id:theCol
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            spacing:Theme.paddingSmall

            PageHeader {
                title:service.artistName
            }
            Image {
                id:fanart1
                anchors.right: parent.right
                anchors.left: parent.left
                source: service.fanart
                height: 300
                fillMode: Image.PreserveAspectFit
                visible: sourceSize.height > 0
            }

            Row {
                spacing:10
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                Label {
                    id:genrelbl
                    text: qsTr("Genre :")
                    font.bold: true
                    color:Theme.highlightColor
                }
                Label {
                    text: service.genres
                    color:Theme.highlightColor
                }
            }
            Row {
                spacing:10
                anchors.left: parent.left
                anchors.leftMargin:Theme.horizontalPageMargin
                Label {
                    id : stylelbl
                    text: qsTr("Style :")
                    font.bold: true
                    color:Theme.highlightColor
                }
                Label {
                    text: service.style
                    color:Theme.highlightColor
                }
            }

            Label {
                id : albumslbl
                //anchors.top: stylelbl.bottom
                //anchors.topMargin: 5 * scalingFactor
                text: qsTr("Available albums : ")
                font.bold: true
                anchors.left: parent.left
                anchors.leftMargin:Theme.horizontalPageMargin
                color:Theme.highlightColor
            }

            Item {
                height:service.albums.length * Theme.itemSizeSmall
                width:parent.width
                Repeater {
                    id:albums
                    model:service.albums
                    delegate: Item {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: Theme.itemSizeSmall
                        y:index * Theme.itemSizeSmall
                        IconButton {
                            width: height
                            height: Theme.itemSizeSmall
                            icon.source:"image://theme/icon-m-add"
                            id:btnaddtopl
                            x:0
                            onClicked: control.addToPlaylist(model.modelData)
                        }
                        IconButton {
                            width: height
                            height: Theme.itemSizeSmall
                            icon.source:"image://theme/icon-m-play"
                            id:btnplay
                            x:width + 5
                            onClicked: control.playFile(model.modelData)
                        }
                        Label {
                            id:albumtext
                            height: Theme.itemSizeSmall
                            verticalAlignment: Text.AlignVCenter
                            x:btnplay.x + btnplay.width+ 5
                            text:model.modelData.label
                            clip:true
                            elide: Text.ElideMiddle
                            width:parent.width - btnplay.width  - btnaddtopl.width - 10
                            MouseArea {
                                anchors.fill: parent
                                onClicked: mediaInformationClicked(model.modelData.filetype,
                                                                   model.modelData.file,
                                                                   model.modelData.label)
                                onPressAndHold: {
                                }
                            }
                        }
                    }
                }

            }
            Label {
                id:lblDesc
                font.bold: true
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Description")
                color:Theme.highlightColor
                anchors.leftMargin: Theme.horizontalPageMargin
            }

            Label {
                id:theText
                text:service.artistDescription
                font.pixelSize: Theme.fontSizeSmall
                anchors.left: parent.left
                anchors.right: parent.right
                wrapMode: Text.WordWrap
                color:Theme.highlightColor
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
            }
        }
    }

    property alias itemId : service.artistId
    property string label
    onItemIdChanged: { service.refresh(); }
    ListContextMenu {
        visible:false
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
    }
    MusicControl {
        id:control
    }

    ArtistInformationService
    {
        id : service
    }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }
}

