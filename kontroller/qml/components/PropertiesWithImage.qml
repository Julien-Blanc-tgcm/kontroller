import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    height: childrenRect.height
    property var properties: []
    property string imageSource: ""
    Column {
        id: theColumn
        anchors.left: parent.left
        width: parent.width * 2 / 3
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
        anchors.left: theColumn.right
        anchors.leftMargin: Theme.paddingSmall
        anchors.right: parent.right
        anchors.rightMargin: Theme.horizontalPageMargin
        height:width
        fillMode: Image.PreserveAspectFit
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
