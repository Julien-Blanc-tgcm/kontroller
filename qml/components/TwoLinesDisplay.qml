import QtQuick 2.0
import Sailfish.Silica 1.0

/*
  Component that will either display a single vertical centered line, or
  two lines, depending if the secondaryLabel property is set

  */

Item { // show label + artist if available
    property string label: ""
    property string secondaryLabel: ""
    property bool active: false
    property bool dimmed: false

    property bool twoLines__: secondaryLabel.length > 0

    height: theLabel.height +
            (twoLines__ ?(theSecondary.height + Theme.paddingSmall):0);

    Label { // else just show item label
        anchors.left: parent.left
        anchors.right: parent.right
        color: active ? Theme.highlightColor:(dimmed?Theme.secondaryColor:Theme.primaryColor)
        id : theLabel
        text: label
        elide: Text.ElideRight
        anchors.top: parent.top
    }

    Label {
        id: theSecondary
        visible: twoLines__
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: theLabel.bottom
        anchors.topMargin: Theme.paddingSmall
        color: active ? Theme.highlightDimmerColor:Theme.secondaryColor
        text: secondaryLabel
        elide: Text.ElideRight
        font.pixelSize: Theme.fontSizeSmall
    }
}
