import QtQuick 2.0
import eu.tgcm 1.0
import "."
Rectangle{
    id:main

    service: null

    color:"#000"

    Text {
        color:Styling.textColor
        font.bold: true
        text: qsTr("Current playlist")
        id:theTitle
        anchors.top: parent.top
        anchors.topMargin: height / 2
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: theme.fontSizeSmall
    }

    Text {
        color:Styling.linkColor
        text: qsTr("Clear")
        id:clear
        anchors.top:parent.top
        anchors.topMargin: height / 2
        anchors.right: parent.right
        anchors.rightMargin: 10
        font.pixelSize: theme.fontSizeSmall
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
            height: theme.fontSizeSmall + 4
            anchors.left: parent.left
            anchors.right: parent.right

            Rectangle {
                visible: thelist.currentIndex === index
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position:0.0;color:Styling.linkColor}
                    GradientStop { position:0.1;color:"#000"}
                    GradientStop { position:0.9;color:"#000"}
                    GradientStop { position:1;color:Styling.linkColor}
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    activateItem(model)
                }
            }
            color: "#000"

            Text {
                id : theText
                text: model.label
                color: service.playlistPosition == index ? "#FFF" : Styling.linkColor
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                font.pixelSize: theme.fontSizeSmall
                anchors.right: remove.left
                elide: Text.ElideRight
                clip:true
            }
            Text {
                id: remove
                text : "X"
                font.pixelSize: theme.fontSizeSmall
                color: Styling.linkColor
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
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

