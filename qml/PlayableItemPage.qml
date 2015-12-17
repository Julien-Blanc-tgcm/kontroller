import QtQuick 2.0
import "."

Item {
    property var service
    property var control
    property var directPlay : []
    property var playableItems:[]

    ListView {
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

        onCurrentIndexChanged:hideSubMenu()
        onContentYChanged:hideSubMenu()

        delegate: Rectangle {
            height:20 * touchScalingFactor
            anchors.left:parent.left;
            anchors.right: parent.right;
            id:rect

            Rectangle {
                visible: thelist.currentIndex == index
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position:0.0;color:Styling.linkColor}
                    GradientStop { position:0.1;color:"#000"}
                    GradientStop { position:0.9;color:"#000"}
                    GradientStop { position:1;color:Styling.linkColor}
                }
            }
            color: thelist.currentIndex == index ? "#111" : "#000"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(thelist.currentIndex !== index)
                        thelist.currentIndex = index
                    else
                        activateItem(model)
                    hideSubMenu()
                }
                onDoubleClicked: activateItem(model)
                onPressAndHold: {
                    thelist.currentIndex = index;
                    theSubMenu.showSubMenu(rect, model, mouseX, mouseY);
                }
            }
            Text {
                id: theText
                text: formatFile(filetype, label)
                color: Styling.linkColor
                font.pixelSize: 12 * scalingFactor
                anchors.verticalCenter: parent.verticalCenter
                anchors.left:parent.left;
                anchors.leftMargin: 5 * scalingFactor;
                anchors.right: parent.right
                anchors.rightMargin:btnplay.width + 5 * scalingFactor
                clip:true
                elide:Text.ElideMiddle
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
                            var found = false;
                            for(var i = 0; i < directPlay.length; ++i)
                            {
                                if(directPlay[i] === service.filesAsList[index].filetype)
                                    found = true;
                            }
                            if(found)
                                activateItem(model);
                        }
                    }
                    onDoubleClicked:
                        activateItem(model)
                    onPressAndHold: {
                        thelist.currentIndex = index;
                        theSubMenu.showSubMenu(rect, model, mouseX, mouseY);
                    }
                }
            }
            Image {
                id:btnplay
                source: "icons/play.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
                width:height
                height:parent.height - 4 * scalingFactor
                visible: {
                    for (var i = 0; i < playableItems.length; i++) {
                        if (playableItems[i] === model.filetype) {
                            return true;
                        }
                    }
                    return false;
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        thelist.currentIndex = index
                        control.playFile(service.filesAsList[thelist.currentIndex])
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
        visible:service.refreshing
        color:"#fff"
        font.pixelSize: 12 * scalingFactor
    }

    ListContextMenu {
        visible: false
        color:"black"
        border.color: Styling.borderColor
        border.width: 2 * scalingFactor
        id:theSubMenu
        onInformationPressed: {
            visible = false;
            var model = service.filesAsList[thelist.currentIndex];
            if(model)
            {
                mediaInformationClicked(model.filetype, model.file, model.label);
            }
        }
        onAddToPlaylistPressed: {
            var model = service.filesAsList[thelist.currentIndex];
            if(model)
            {
                if(model.filetype === "directory" || model.filetype === "album" || model.filetype === "song" || model.filetype === "file")
                    control.addToPlaylist(model)
            }
            visible = false
        }
        onPlayPressed: {
            var model = service.filesAsList[thelist.currentIndex];
            if(model)
            {
                if(model.filetype === "directory" || model.filetype === "album" || model.filetype === "song" || model.filetype === "file")
                    control.playFile(model)
            }
            visible = false
        }
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

