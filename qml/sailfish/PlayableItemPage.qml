import QtQuick 2.0
import Sailfish.Silica 1.0
import "."

Item {
    property var service
    property var control
    property var directPlay : []
    property var playableItems:[]

    property alias header : thelist.header

    SilicaListView {
        id:thelist
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: service.filesAsList
        clip:true
        spacing:1
        currentIndex: -1
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

        delegate: ListItem {
            width: ListView.view.width
            contentHeight:Theme.itemSizeSmall
            id:currentItem
            Label {
                text : formatFile(filetype, label)
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: btnplay.left
                anchors.verticalCenter: parent.verticalCenter
                clip:true
                elide: Text.ElideRight
            }
            IconButton {
                id:btnplay
                icon.source: "image://theme/icon-m-play"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 5
                visible: {
                    for (var i = 0; i < playableItems.length; i++) {
                        if (playableItems[i] === model.filetype) {
                            return true;
                        }
                    }
                    return false;
                }
                onClicked: control.playFile(service.filesAsList[index])
            }
            onClicked: activateItem(model)
            property var listCurrentItem: model
            menu: ContextMenu {
                Repeater {
                    model:getMenuItems(listCurrentItem)
                    delegate: MenuItem{
                        text:modelData.text
                        onClicked: execute(modelData.type, listCurrentItem)
                    }
                }
            }
        }
    }
    Text {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: "Please wait..."
        visible:service.refreshing
    }

    function getMenuItems(item)
    {
        switch(item.filetype)
        {
        case "album":
        case "video":
        case "movie":
        case "season":
        case "episode":
            return [
                        {text: qsTr("Add to playlist"), type: "addtoplaylist"},
                        {text:qsTr("Play immediately"), type: "play" },
                        {text: qsTr("View information"), type: "information"}
                    ];
        case "audiodirectory":
        case "song":
            return [
                        { text:qsTr("Add to playlist"), type:"addtoplaylist"},
                        { text:qsTr("Play immediately"), type:"play"}
                    ];
        case "artist":
            return [
                        { text:qsTr("View information"), type:"information"}
                    ];
        }
       // console.log(item.filetype);
    }

    function execute(action, item)
    {
        console.log("Executing " + action + " on " + item.file);
    }

    property var currentMenuItem

    function hideSubMenu()
    {
        theSubMenu.visible = false;
    }

    function activateItem(modelItem)
    {
        if(modelItem.filetype === "file")
            control.playFile(service.filesAsList[modelItem.index]);
        else if(modelItem.filetype === "song")
            control.playFile(service.filesAsList[modelItem.index]);
        else
            mediaClicked(modelItem.filetype, modelItem.file, modelItem.label)
    }

    function formatFile(filetype, label)
    {
         if(filetype === "file")
             return label;
         if(filetype === "directory")
             return "[" + label + "]";
         return label;
    }
}

