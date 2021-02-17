import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."
import "utils.js" as Utils
Page{
    id:main

    property var service;

    SilicaListView {
        header:PageHeader {
            title:qsTr("Current playlist")
        }

        id:thelist
        anchors.fill:parent
        model: service?service.items:[]
        clip:true
        currentIndex: service?service.playlistPosition:-1

        delegate : ListItem {
            contentHeight: Theme.itemSizeMedium
            width: parent.width
            property bool active : service.playlistPosition === index
            Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                id: time
                text: Utils.formatMsecTime(model.modelData.duration * 1000)
                anchors.verticalCenter: parent.verticalCenter
                width: Theme.itemSizeMedium
                font.pixelSize: Theme.fontSizeExtraLarge
                visible: model.modelData.duration !== 0 // some kodi versions are buggy and return 0, so don't show that
                color:  active?Theme.highlightDimmerColor:Theme.secondaryColor
            }

            Item { // show label + artist if available
                anchors.left: time.visible?time.right:parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                anchors.right: parent.right
                visible: model.modelData.artist.length !== 0
                anchors.verticalCenter: parent.verticalCenter
                height: theLabel.height + theArtist.height + Theme.paddingSmall

                Label { // else just show item label
                    anchors.left: parent.left
                    anchors.right: parent.right
                    color: active ? Theme.highlightColor:Theme.primaryColor
                    id : theLabel
                    text: model.modelData.label
                    elide: Text.ElideRight
                    anchors.top: parent.top
                }

                Label {
                    id: theArtist
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: theLabel.bottom
                    anchors.topMargin: Theme.paddingSmall
                    color: active ? Theme.highlightDimmerColor:Theme.secondaryColor
                    text: model.modelData.artist
                    elide: Text.ElideRight
                    font.pixelSize: Theme.fontSizeSmall
                }
            }

            Label { // else just show item label (for movies)
                anchors.left: time.visible?time.right:parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                color: active ? Theme.highlightColor:Theme.primaryColor
                id : theText
                text: model.modelData.label
                elide: Text.ElideRight
                anchors.verticalCenter: parent.verticalCenter
                visible: model.modelData.artist.length === 0
            }

            menu: ContextMenu {
                MenuItem {
                    text:qsTr("Remove")
                    onClicked: service.removeElement(model.index)
                }
                hasContent: service.playlistPosition !== index
            }
            onClicked: activateItem(model)

        }
    }

    function activateItem(modelItem)
    {
        service.switchToItem(modelItem.index)
    }

    function clear()
    {
        service.clearPlaylist()
    }

    Connections {
        target: appClient.playerService
        onPlayersChanged: {
            if(appClient.playerService.players.length > 0)
            {
                service = appClient.playerService.players[0].playlistService
            }
            else
                service = null
        }
    }
    Component.onCompleted: {
        if(appClient.playerService.players.length > 0)
            service = appClient.playerService.players[0].playlistService
    }

}

