import QtQuick 2.0

import eu.tgcm 1.0

Item {
    id:main
    property alias browsingMode: musicService.browsingMode
    property alias browsingValue: musicService.browsingValue
    property alias label: musicService.label
    signal fileClicked(string file)
    signal mediaClicked(string filetype, string file, string label)
    signal songClicked(string song)
    signal playDirectoryAsked(string dir)

    MusicService {
        id: musicService
    }
    property var service : musicService

    ListView {
        id:thelist
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: musicService.filesAsList
        clip:true
        spacing:1
        currentIndex: -1
        visible: !musicService.refreshing

        onCurrentIndexChanged:hideSubMenu()
        onContentYChanged:hideSubMenu()

        delegate: Rectangle {
            height:theText.height * 2
            anchors.left:parent.left;
            anchors.right: parent.right;
            id:rect

            Rectangle {
                visible: thelist.currentIndex == index
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position:0.0;color:"#00B"}
                    GradientStop { position:0.1;color:"#000"}
                    GradientStop { position:0.9;color:"#000"}
                    GradientStop { position:1;color:"#00B"}
                }
            }
            color: thelist.currentIndex == index ? "#111" : "#000"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    thelist.currentIndex = index
                    hideSubMenu()
                }
                onDoubleClicked: activateItem(model)
                onPressAndHold: {
                    thelist.currentIndex = index;
                    showSubMenu(rect, model, mouseX, mouseY);
                }
            }
            Text {
                id: theText
                text: formatFile(filetype, label)
                color: "#0000DD"
                font.pixelSize: 12 * scalingFactor
                anchors.verticalCenter: parent.verticalCenter
                anchors.left:parent.left;
                anchors.leftMargin: 10 * scalingFactor;
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(theSubMenu.visible)
                        {
                            hideSubMenu()
                            thelist.currentIndex = index
                        }
                        else
                        {
                            thelist.currentIndex = index
                            activateItem(model);
                        }
                    }
                    onPressAndHold: {
                        thelist.currentIndex = index;
                        showSubMenu(rect, model, mouseX, mouseY);
                    }
                }
            }
            Image {
                source: "icons/play.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
                width:height
                height:parent.height - 4 * scalingFactor
                visible: filetype === "directory" || filetype === "album" || filetype === "song" || filetype === "file"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        thelist.currentIndex = index
                        musicService.playFile(musicService.filesAsList[thelist.currentIndex])
                        parent.source = ""
                    }
                }
            }
        }
    }
    Text {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: "Please wait..."
        visible:musicService.refreshing
        color:"#fff"
        font.pixelSize: 12 * scalingFactor
    }

    ListContextMenu {
        visible: false
        color:"black"
        border.color: "#00b"
        border.width: 2 * scalingFactor
        id:theSubMenu
        onInformationPressed: {
            visible = false
        }
        onAddToPlaylistPressed: {
            var model = musicService.filesAsList[thelist.currentIndex];
            if(model)
            {
                if(model.filetype === "directory" || model.filetype === "album" || model.filetype === "song" || filetype === "file")
                    musicService.addToPlaylist(model)
            }
            visible = false
        }
        onPlayPressed: {
            var model = musicService.filesAsList[thelist.currentIndex];
            if(model)
            {
                if(model.filetype === "directory" || model.filetype === "album" || model.filetype === "song" || filetype === "file")
                    musicService.playFile(model)
            }
            visible = false
        }
    }

    property var currentMenuItem

    function showSubMenu(item, model, x, y)
    {
        currentMenuItem = model
        var coord = main.mapFromItem(item, x, y);
        theSubMenu.x = coord.x;
        theSubMenu.y = coord.y;
        if(model.filetype === "directory")
            theSubMenu.state = "audiodirectory";
        else if(model.filetype === "album")
            theSubMenu.state = "album";
        else if(model.filetype === "artist")
            theSubMenu.state = "artist";
        else if(model.filetype === "song" || model.filetype === "file")
            theSubMenu.state = "song";
        else if(model.filetype === "directory")
            theSubMenu.state = "directory";
        else
            return;
        theSubMenu.visible = true
    }

    function hideSubMenu()
    {
        theSubMenu.visible = false;
    }

    Component.onCompleted: musicService.refresh()

    function activateItem(modelItem)
    {
        if(modelItem.filetype === "file")
            fileClicked(model.file);
        else if(modelItem.filetype === "song")
            songClicked(modelItem.file);
        else
            mediaClicked(modelItem.filetype, modelItem.file, label + "/" + modelItem.label)
    }

    function refresh()
    {
        musicService.refresh()
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
