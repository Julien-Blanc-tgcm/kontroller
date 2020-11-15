import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."
Page {
    id:main
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    property var pageClient: appClient

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
                spacing: Theme.paddingMedium
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
                spacing: Theme.paddingMedium
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
                text: qsTr("Available albums:")
                font.bold: true
                anchors.left: parent.left
                anchors.leftMargin:Theme.horizontalPageMargin
                color:Theme.highlightColor
            }

            SilicaListView {
                height:childrenRect.height
                id:albums
                model:service.albums
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 0
                delegate: ListItem {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    contentHeight: Theme.itemSizeMedium
                   // height: Theme.itemSizeSmall
//                        IconButton {
//                            width: height
//                            height: Theme.itemSizeSmall
//                            icon.source:"image://theme/icon-m-add"
//                            id:btnaddtopl
//                            x:0
//                            onClicked: control.addToPlaylist(model.modelData)
//                        }
                    Image {
                        x: Theme.horizontalPageMargin
                        visible:model.modelData.thumbnail.length > 0
                        source:model.modelData.thumbnail
                        height: Theme.iconSizeMedium
                        width: height
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectFit
                        id:img
                    }
                    IconButton {
                        x: Theme.horizontalPageMargin
                        visible: !img.visible
                        icon.source: "image://theme/icon-m-media-albums"
                        anchors.verticalCenter: parent.verticalCenter
                        height: icon.height
                        width: icon.width
                        id: theIcon
                    }

                    Label {
                        id:albumtext
                        //height: Theme.itemSizeSmall
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:model.modelData.label
                        wrapMode: Text.Wrap
                        //elide: Text.ElideMiddle
                        anchors.left: img.visible?img.right:theIcon.right
                        anchors.leftMargin: Theme.paddingMedium
                        anchors.right: btnplay.left
                        anchors.rightMargin: Theme.paddingMedium
                    }
                    IconButton {
                        width: icon.width
                        height: icon.height
                        icon.source:"image://theme/icon-m-play"
                        id:btnplay
                        anchors.right: parent.right
                        anchors.rightMargin: Theme.horizontalPageMargin
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: control.playFile(model.modelData)
                    }
                    menu: ContextMenu {
                        MenuItem {
                            text: qsTr("View information")
                            onClicked: mediaInformationClicked(model.modelData)
                        }
                        MenuItem {
                            text: qsTr("Add to playlist")
                            onClicked: control.addToPlaylist(model.modelData)
                        }
                        MenuItem {
                            text: qsTr("Play immediately")
                            onClicked: control.playFile(model.modelData)
                        }
                        MenuItem {
                            text: qsTr("Download")
                            onClicked: pageClient.downloadService.addAlbum(model.modelData.label, model.modelData.id)
                        }
                    }
                    onClicked: mediaInformationClicked(model.modelData)
                }
            }
            Label {
                id:lblDesc
                font.bold: true
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Description:")
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
    ListContextMenu {
        visible:false
        id:theSubMenu
        onInformationPressed:
        {
            visible = false
            if(currentModel)
                mediaInformationClicked(currentModel)
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
        client: pageClient
    }

    ArtistInformationService
    {
        id : service
        client: pageClient
    }

    signal mediaInformationClicked(var file)
    function refresh() { service.refresh(); }
    Component.onCompleted: {
        refresh()
    }
}

