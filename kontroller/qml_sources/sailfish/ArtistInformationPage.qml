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
                text: qsTr("Available albums : ")
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
                spacing: Theme.paddingSmall
                delegate: ListItem {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    contentHeight: Math.max(img.height, albumtext.height)
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
                        anchors.left: parent.left
                        anchors.leftMargin: Theme.horizontalPageMargin
                        visible:model.modelData.thumbnail.length > 0
                        source:model.modelData.thumbnail
                        height: Theme.itemSizeSmall
                        width: height
                        anchors.verticalCenter: parent.verticalCenter
                        id:img
                    }

                    Label {
                        id:albumtext
                        //height: Theme.itemSizeSmall
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text:model.modelData.label
                        wrapMode: Text.Wrap
                        //elide: Text.ElideMiddle
                        anchors.left: img.visible?img.right:parent.left
                        anchors.leftMargin: img.visible?Theme.paddingSmall:Theme.horizontalPageMargin
                        anchors.right: btnplay.left
                        anchors.rightMargin: Theme.paddingSmall
                    }
                    IconButton {
                        width: height
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
                            onClicked: mediaInformationClicked(model.modelData.filetype,
                                                               model.modelData.file,
                                                               model.modelData.label)
                        }

                        MenuItem {
                            text: qsTr("Add to playlist")
                            onClicked: control.addToPlaylist(model.modelData)
                        }
                        MenuItem {
                            text: qsTr("Play immediately")
                            onClicked: control.playFile(model.modelData)
                        }
                    }
                    onClicked: mediaInformationClicked(model.modelData.filetype,
                                                       model.modelData.file,
                                                       model.modelData.label)
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
        client: pageClient
    }

    ArtistInformationService
    {
        id : service
        client: pageClient
    }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }
    Component.onCompleted: {
        refresh()
    }
}

