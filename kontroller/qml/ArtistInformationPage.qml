import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."
import "./components"
import "./utils.js" as Utils

Page {
    id:main
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    property var pageClient: appClient

    SilicaFlickable {
        anchors.fill:parent
        contentHeight:theCol.childrenRect.height
        visible: !service.refreshing

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

            PropertiesWithImage {
                anchors.left: parent.left
                anchors.right: parent.right
                properties: [
                    qsTr("<b>Genre:</b> %1").arg(service.genres),
                    qsTr("<b>Style:</b> %1").arg(service.style)
                ]
                imageSource: service.thumbnail
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
                visible: !service.refreshingAlbums
                delegate: ListItem {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    contentHeight: Theme.itemSizeMedium
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
                            text: qsTr("View info")
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
            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                visible:service.refreshingAlbums
                running: visible
                size: BusyIndicatorSize.Medium
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
            Image {
                id:fanart1
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                source: service.fanart
                fillMode: Image.PreserveAspectFit
                visible: sourceSize.height > 0
            }
        }
    }

    BusyIndicator {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        visible:service.refreshing
        running: visible
        size: BusyIndicatorSize.Large
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

