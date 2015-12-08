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
        color:appstyle.textColor
        font.bold: true
        text: qsTr("Current playlist")
        id:theTitle
        anchors.top: parent.top
        anchors.topMargin: height / 2
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 12 * scalingFactor
    }

    Text {
        color:appstyle.textColor
        text: qsTr("Clear")
        id:clear
        anchors.top:parent.top
        anchors.topMargin: height / 2
        anchors.right: parent.right
        anchors.rightMargin: 10
        MouseArea {
            anchors.fill: parent
            onClicked: main.clear()
        }
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
        currentIndex: service.playlistPosition

        delegate : Rectangle {
            height: theText.height * 2
            anchors.left: parent.left
            anchors.right: parent.right

            Rectangle {
                visible: thelist.currentIndex === index
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position:0.0;color:appstyle.textColor}
                    GradientStop { position:0.1;color:"#000"}
                    GradientStop { position:0.9;color:"#000"}
                    GradientStop { position:1;color:appstyle.textColor}
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
                color: service.playlistPosition == index ? "#FFF" : appstyle.textColor
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
            Text {
                id: remove
                text : "X"
                color: appstyle.textColor
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                MouseArea {
                    anchors.fill: parent
                    onClicked: service.removeElement(model.index)
                }
            }
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

}

