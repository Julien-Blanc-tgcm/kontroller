import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."
import "./components"
import "utils.js" as Utils

Page {
    id:main
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    SilicaFlickable {
        clip:true
        anchors.fill: parent
        contentHeight: txtTitle.height + col.childrenRect.height + Theme.paddingMedium
        visible: !service.refreshing

        PageHeader {
            title:service.name
            id:txtTitle
            wrapMode: Text.WordWrap
            clip:true
        }

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

        VerticalScrollDecorator {}

        Column {
            id:col
            anchors.top:txtTitle.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            spacing:Theme.paddingSmall

            PropertiesWithImage {
                anchors.left: parent.left
                anchors.right: parent.right
                properties: [
                    qsTr("<b>Artists:</b> %1").arg(service.artists[0]),
                    qsTr("<b>Year:</b> %1").arg((service.year !== 0)? service.year:""),
                    qsTr("<b>Label:</b> %1").arg(service.label),
                    qsTr("<b>Genre:</b> %1").arg(Utils.formatArray(service.genres)),
                ]
                imageSource: service.thumbnail
            }

            Button {
                text:qsTr("Play album")
                onClicked: service.playFile()
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                id : songslbl
                text: qsTr("Songs:")
                font.bold: true
                x:Theme.horizontalPageMargin
                color:Theme.highlightColor
            }

            SilicaListView {
                width:parent.width
                height: childrenRect.height
                model:service.songs
                visible: !service.refreshingSongs
                delegate: ListItem {
                    contentHeight: Theme.itemSizeSmall
                    anchors.left: parent.left
                    anchors.right: parent.right
                    Label {
                        height:Theme.itemSizeSmall
                        verticalAlignment: Text.AlignVCenter
                        anchors.left: parent.left
                        anchors.leftMargin: Theme.horizontalPageMargin
                        text:model.modelData.label
                        clip:true
                        elide: Text.ElideMiddle
                        anchors.right: btnplay.left
                        anchors.rightMargin: Theme.horizontalPageMargin
                    }
                    IconButton {
                        icon.source:"image://theme/icon-m-play"
                        id:btnplay
                        anchors.right: parent.right
                        anchors.rightMargin: Theme.horizontalPageMargin
                        onClicked: control.playFile(model.modelData)
                    }
                    menu: ContextMenu {
                        MenuItem {
                            text: qsTr("Add to playlist")
                            onClicked: control.addToPlaylist(model.modelData)
                        }
                        MenuItem {
                            text: qsTr("Play immediately")
                            onClicked: control.playFile(model.modelData)
                        }
                    }
                }
            }
            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                visible:service.refreshingSongs
                running: visible
                size: BusyIndicatorSize.Medium
            }

            Label {
                id:theText
                color:Theme.highlightColor
                wrapMode: Text.WordWrap
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
                font.bold: true
                text:qsTr("Description:")
            }
            Label {
                color:Theme.highlightColor
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.WordWrap
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
                text:service.description
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

    property alias itemId : service.albumId
    property string label

    AlbumInformationService
    {
        id : service
        client: appClient
    }
    MusicControl
    {
        id: control
        client: appClient
    }
    Component.onCompleted: service.refresh()

    signal mediaInformationClicked(var file)
    function refresh() { service.refresh(); }
}


