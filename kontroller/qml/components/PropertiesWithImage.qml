import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    height: childrenRect.height
    property var properties: []
    property string imageSource: ""
    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: parent.width / 3
        anchors.top: parent.top
        Repeater {
            id:rep
            model: properties
            delegate: Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.right: parent.right
                text: model.modelData
                verticalAlignment: Text.AlignVCenter
                color:Theme.highlightColor
                wrapMode: Text.Wrap
                clip: true
            }
        }
    }

    Image {
        id: fanart
        height:parent.width / 3
        width:parent.width / 3
        fillMode: Image.PreserveAspectFit
        anchors.right: parent.right
        anchors.rightMargin: Theme.horizontalPageMargin
        anchors.top:parent.top
        source: imageSource
        BusyIndicator {
            anchors.centerIn: parent
            size: BusyIndicatorSize.Small
            running: visible
            visible: fanart.status == Image.Loading
        }
    }
}
