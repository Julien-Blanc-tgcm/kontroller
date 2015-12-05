import QtQuick 2.0

import eu.tgcm 1.0

Item {
    property alias browsingMode: videoService.browsingMode
    property alias browsingValue: videoService.browsingValue
    property alias label: videoService.label
    signal fileClicked(string file)
    signal mediaClicked(string filetype, string file, string label)

    VideoService {
        id: videoService
    }

    ListView {
        id:thelist
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: videoService.filesAsList
        clip:true
        spacing:1
        focus:true
        currentIndex: -1
        visible: !videoService.refreshing

        delegate: Rectangle {
            height:theText.height * 2
            anchors.left:parent.left;
            anchors.right: parent.right;


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
                }
                onDoubleClicked: {
                    activateItem(model)
                }
            }
            Text {
                id: theText
                text: formatFile(filetype, label)
                color: "#0000DD"

                anchors.verticalCenter: parent.verticalCenter
                anchors.left:parent.left;
                anchors.leftMargin: 10;
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        thelist.currentIndex = index
                        activateItem(model);
                    }
                }
            }
            Image {
                source: "icons/play.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                width:height
                height:theText.height
                visible: filetype === "directory" || filetype === "album" || filetype === "song" || filetype === "file"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        thelist.currentIndex = index
                        videoService.playFile(videoService.filesAsList[thelist.currentIndex])
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
        visible:videoService.refreshing
        color:"#fff"
    }

    Component.onCompleted: videoService.refresh()

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
        videoService.refresh()
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
