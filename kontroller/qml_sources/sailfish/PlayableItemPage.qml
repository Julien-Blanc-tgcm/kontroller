import QtQuick 2.0
import Sailfish.Silica 1.0
import "."

Item {
    property var service
    property var control
    property var directPlay : []
    property var playableItems:[]
    property string mediaType:""

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
            anchors.horizontalCenter: parent.horizontalCenter
            width: childrenRect.width
            spacing: Theme.paddingSmall
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
        spacing:Theme.paddingSmall
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
            anchors.left: parent.left
            anchors.right: parent.right
            id:currentItem
            contentHeight: Math.max(img.height, lbl.height)

            Image {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                visible:modelData.thumbnail.length > 0
                source:modelData.thumbnail
                height: Theme.itemSizeSmall
                width: height
                anchors.verticalCenter: parent.verticalCenter
                id:img
                fillMode: Image.PreserveAspectFit
            }
            IconButton {
                id:theIcon
                visible: getSourceIcon(modelData.icon).length > 0
                icon.source: getSourceIcon(modelData.icon)
                height:icon.height
                width:height
                clip:true
                icon.fillMode: Image.PreserveAspectFit
                Component.onCompleted: {
                    if(img.visible)
                    {
                        anchors.bottom = img.bottom;
                        anchors.left = img.left
                        icon.height = Theme.iconSizeExtraSmall;
                        icon.width = Theme.iconSizeExtraSmall;
                    }
                    else
                    {
                        anchors.leftMargin = Theme.horizontalPageMargin
                        anchors.verticalCenter = parent.verticalCenter;
                        anchors.left = parent.left;
                    }
                }
            }

            Label {
                id: lbl
                text : formatFile(modelData.filetype, modelData.label)
                anchors.left: img.visible ? img.right: (theIcon.visible?theIcon.right:parent.left)
                anchors.leftMargin: (img.visible || theIcon.visible) ? Theme.paddingSmall : Theme.horizontalPageMargin
                anchors.right: btnplay.visible ? btnplay.left : parent.left
                anchors.rightMargin: btnplay.visible ? Theme.paddingSmall : Theme.horizontalPageMargin
                anchors.verticalCenter: parent.verticalCenter
                wrapMode: Text.Wrap
            }
            IconButton {
                id:btnplay
                icon.source: "image://theme/icon-m-play"
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
                anchors.verticalCenter: parent.verticalCenter
                visible: {
                    for (var i = 0; i < playableItems.length; i++) {
                        if (playableItems[i] === modelData.filetype) {
                            return true;
                        }
                    }
                    return false;
                }
                onClicked:
                {
                    control.playFile(modelData)
                }
            }
            onClicked: activateItem(modelData)
            menu: ContextMenu {
                Repeater {
                    model:getMenuItems(modelData)
                    delegate: MenuItem{
                        text: modelData.text
                        onClicked: execute(modelData.type, modelData.item)
                    }
                }
                hasContent: getMenuItems(modelData).length > 0
            }
        }
    }
    BusyIndicator {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        visible:service.refreshing && !service.inputRequested
        running: visible
        size: BusyIndicatorSize.Large
    }

    function getMenuItems(playableItem)
    {
        if(playableItem == null)
            return [];
        switch(playableItem.filetype)
        {
        case "album":
        case "video":
        case "movie":
        case "season":
        case "episode":
            return [
                        {text: qsTr("Add to playlist"), type: "addtoplaylist", item: playableItem},
                        {text: qsTr("Play immediately"), type: "play", item: playableItem},
                        {text: qsTr("View information"), type: "information", item: playableItem}
                    ];
        case "audiodirectory":
        case "directory":
        case "song":
        case "file":
            return [
                        { text:qsTr("Add to playlist"), type:"addtoplaylist", item: playableItem},
                        { text:qsTr("Play immediately"), type:"play", item: playableItem},
                        { text:qsTr("Download"), type: "download", item: playableItem}
                    ];
        case "artist":
            return [
                        { text:qsTr("View information"), type:"information", item : playableItem}
                    ];
        }
        return [  ];
//        console.log(item.filetype);
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
            control.playFile(item.file);
        }
        else if(action === "addtoplaylist")
        {
            control.addToPlaylist(item.file);
        }
        else if(action === "download")
        {
            if(item.filetype === "file" )
            {
                appClient.downloadService.addFile(item.file, mediaType)
            }
            else if(item.filetype === "directory")
            {
                appClient.downloadService.addFolder(item.file, mediaType)
            }
            else
            {
                console.log("Download action on " + item.filetype + " not implemented yet");
            }
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
            control.playFile(modelItem);
        else if(modelItem.filetype === "song")
            control.playFile(modelItem);
        else if(modelItem.filetype === "musicvideo")
            control.playFile(modelItem);
        else
            mediaClicked(modelItem.filetype, modelItem.file, modelItem.label)
    }

    function formatFile(filetype, label)
    {
        return label;
    }

    function getSourceIcon(icon)
    {
        if(icon === "music")
            return "image://theme/icon-m-music";
        if(icon === "folder")
            return "image://theme/icon-m-folder";
        if(icon === "video")
            return "image://theme/icon-m-file-video";
        if(icon === "artists")
            return "image://theme/icon-m-media-artists";
        if(icon === "albums")
            return "image://theme/icon-m-media-albums";
        if(icon === "genres")
            return "image://theme/icon-m-media-radio"
        if(icon === "songs")
            return "image://theme/icon-m-media-songs";
        if(icon === "movies")
            return "image://theme/icon-m-media";
        if(icon === "tvshows")
            return "image://theme/icon-m-media-playlists";
        if(icon === "clips")
            return "image://theme/icon-m-media-songs";
        return "";
    }
}

