import QtQuick 2.0
import eu.tgcm 1.0

Rectangle{
    id:main

    property alias playlistId : service.playlistId
    property alias playlistType: service.playlistType
    property alias playlistPosition: service.playlistPosition

    color:"#000"

    PlaylistService
    {
        id :service
    }

    Text {
        color:"#00b"
        font.bold: true
        text: qsTr("Current playlist")
        id:theTitle
        anchors.top: parent.top
        anchors.topMargin: height / 2
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ListView {
        id:thelist
        anchors.top: theTitle.bottom
        anchors.topMargin: theTitle.anchors.topMargin
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        model: service.items
        clip:true
        spacing:1
        focus:true
        currentIndex: service.playlistPosition

        delegate : Rectangle {
            height: theText.height * 2
            anchors.left: parent.left
            anchors.right: parent.right

            Rectangle {
                visible: thelist.currentIndex === index
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position:0.0;color:"#00B"}
                    GradientStop { position:0.1;color:"#000"}
                    GradientStop { position:0.9;color:"#000"}
                    GradientStop { position:1;color:"#00B"}
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    thelist.currentIndex = index
                }
                onDoubleClicked: {
                    activateItem(model)
                }
            }
            color: "#000"

            Text {
                id : theText
                text: model.label
                color: service.playlistPosition == index ? "#FFF" : "#0000DD"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
        }
    }

    function activateItem(modelItem)
    {
        service.switchToItem(modelItem.index)
    }

}

