import QtQuick 2.0
import Sailfish.Silica 1.0
import "."
import "components"

SilicaFlickable {
    id: __rootItem
    property var service
    property var control
    property var playableItems:[]
    property string mediaType:""
    property string title : ""

    FilteredModel {
        id: theModel
        filter: ""
        sourceModel: service.filesAsList
        properties: ["label"]
        type: 1 // FilterType.Contains
    }

    VerticalScrollDecorator {flickable: __rootItem}
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


    Rectangle {
        id: inputMain
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

    Item {
        anchors.left: parent.left
        id: searchBar
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: Theme.horizontalPageMargin
        anchors.right: parent.right
        height: childrenRect.height
        z: 1
        PageHeader {
            id: _header
            title: __rootItem.title
        }
        Item {
            visible: theModel.sourceModel.length > 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: _header.bottom
            height: Math.max(_icon.height, _searchBox.height)
            Icon {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                source: "image://theme/icon-m-search"
                id:_icon
                highlighted: _searchBox.activeFocus
            }
            TextInput {
                anchors.left: _icon.right
                anchors.right: _cancel.visible?_cancel.left:parent.right
                anchors.rightMargin: _cancel.visible?Theme.paddingLarge:0
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Theme.fontSizeMedium
                color: activeFocus?Theme.highlightColor:Theme.primaryColor
                id: _searchBox
                property string __initialText : qsTr("Search")
                text: __initialText
                EnterKey.iconSource: "image://theme/icon-m-search"
                EnterKey.onClicked: {
                    if(text == "")
                    {
                        theModel.filter = ""
                        text = __initialText
                    }
                    else
                        theModel.filter = text
                }
                onActiveFocusChanged: {
                    if(activeFocus && text == __initialText)
                        text = "";
                }
            }
            IconButton {
                id: _cancel
                icon.source: "image://theme/icon-m-clear"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                onClicked: {
                    _searchBox.text = _searchBox.__initialText
                    theModel.filter = ""
                }
            }
        }
    }

    SilicaListView {
        id:thelist
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchBar.bottom
        anchors.bottom: parent.bottom
        clip:true
        visible: !service.refreshing

        model: theModel.filteredModel //service.filesAsList
        spacing:0
        currentIndex: -1

        delegate: ListItem {
            anchors.left: parent.left
            anchors.right: parent.right
            id:currentItem
            contentHeight: Math.max(Theme.itemSizeMedium, twoLinesDisplay.height)

            Image {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                visible:modelData.thumbnail.length > 0
                source:modelData.thumbnail
                height: Theme.iconSizeMedium
                width: height
                anchors.verticalCenter: parent.verticalCenter
                id:img
                fillMode: Image.PreserveAspectFit
            }
            IconButton {
                id:theIcon
                visible: getSourceIcon(modelData.icon).length > 0 && (service.alwaysDisplayIcons || !img.visible)
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

            TwoLinesDisplay {
                id:twoLinesDisplay
                anchors.left: img.visible ? img.right: (theIcon.visible?theIcon.right:parent.left)
                anchors.leftMargin: (img.visible || theIcon.visible) ? Theme.paddingMedium : Theme.horizontalPageMargin
                anchors.right: btnplay.visible ? btnplay.left : parent.right
                anchors.rightMargin: btnplay.visible ? Theme.paddingMedium : Theme.horizontalPageMargin
                anchors.verticalCenter: parent.verticalCenter
                label: model.modelData.label
                secondaryLabel: model.modelData.secondaryLabel
                dimmed: modelData.played
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

    Label {
        text: qsTr("No items found!")
        anchors.centerIn: parent
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: Theme.horizontalPageMargin
        horizontalAlignment: Text.AlignHCenter
        clip:true
        wrapMode: Text.WordWrap
        color: Theme.highlightColor
        visible: theModel.filteredModel.length === 0 && !service.refreshing
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
        if(playableItem === null)
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
                        {text: qsTr("View information"), type: "information", item: playableItem},
                        { text:qsTr("Download"), type: "download", item: playableItem}
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
            mediaInformationClicked(item)
        }
        else if(action === "play")
        {
            control.playFile(item);
        }
        else if(action === "addtoplaylist")
        {
            control.addToPlaylist(item);
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
            else if(item.filetype === "album")
            {
                appClient.downloadService.addAlbum(item.label, item.id)
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
            mediaClicked(modelItem)
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
        if(icon === "artists" || icon === "artist")
            return "image://theme/icon-m-media-artists";
        if(icon === "albums" || icon === "album")
            return "image://theme/icon-m-media-albums";
        if(icon === "genres")
            return "image://theme/icon-m-media-radio"
        if(icon === "songs" || icon === "song")
            return "image://theme/icon-m-media-songs";
        if(icon === "movies" || icon === "movie")
            return "image://assets/icons/icon-m-movie";
        if(icon === "tvshows" || icon === "tvshow")
            return "image://assets/icons/icon-m-tv";
        if(icon === "musicvideos" || icon === "musicvideo")
            return "image://assets/icons/icon-m-musicvideo";
        if(icon === "recentlyaddedalbums")
            return "image://assets/icons/icon-m-recently-added";
        if(icon === "recentlyplayedalbums")
            return "image://assets/icons/icon-m-recently-played";
        if(icon === "randomalbums")
            return "image://assets/icons/icon-m-random";
        if(icon === "image")
            return "image://theme/icon-m-file-image";
        return "";
    }
}

