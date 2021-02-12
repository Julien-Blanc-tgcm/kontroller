import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."
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
            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                color: (service.playlistPosition === index) ? Theme.highlightColor:Theme.primaryColor
                id : theText
                text: model.modelData.label
                elide: Text.ElideRight
                anchors.verticalCenter: parent.verticalCenter
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

