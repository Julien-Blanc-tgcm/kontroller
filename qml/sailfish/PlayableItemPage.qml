import QtQuick 2.0
import Sailfish.Silica 1.0
import "."

Item {
    property var service
    property var control
    property var directPlay : []
    property var playableItems:[]

    property alias header : thelist.header

    Rectangle {
        id: inputMain
        z: 10
        anchors.top:parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        clip:true
        visible: service.inputRequested?true:false
        color: "transparent"
        Label {
            id: inputTitle
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: Theme.itemSizeMedium
            text: service.inputTitle
            color: Theme.primaryColor
        }
        TextField {
            id: textInput
            anchors.top: inputTitle.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingMedium
            anchors.rightMargin: Theme.paddingMedium
            placeholderText: service.inputValue
            color: Theme.primaryColor
            height: Theme.itemSizeLarge
            label: qsTr("Input text")
        }
        Row {
            anchors.top : textInput.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingMedium
            anchors.rightMargin: Theme.paddingMedium
            Button {
                text: qsTr("Cancel")
                onClicked: { service.inputBack(); }
            }
            Button {
                text: qsTr("OK")
                onClicked: { service.inputText(textInput.text); }
            }
        }
    }

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
            Image {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                visible:model.thumbnail.length > 0
                source:model.thumbnail
                height:parent.height - 2 * Theme.paddingSmall
                width: height * sourceSize.width / sourceSize.height
                anchors.verticalCenter: parent.verticalCenter
                id:img
            }

            Label {
                text : formatFile(filetype, label)
                anchors.left: img.visible ? img.right :parent.left
                anchors.leftMargin: img.visible ? Theme.paddingSmall : Theme.horizontalPageMargin
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
        case "directory":
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
        console.log(item.filetype);
        return [];
    }

    function execute(action, item)
    {
        if(action === "information")
        {
            var modelItem = service.filesA
            mediaInformationClicked(item.filetype, item.file, item.label)
        }
        else if(action === "play")
        {
            control.playFile(service.filesAsList[item.index]);
        }
        else if(action === "addtoplaylist")
        {
            control.addToPlaylist(service.filesAsList[item.index]);
        }
        else
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
        else if(modelItem.filetype === "musicvideo")
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

