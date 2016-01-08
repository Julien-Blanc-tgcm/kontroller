import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "utils.js" as Utils
import "."
Page {
    id:main
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    SilicaFlickable {

        PageHeader {
            id:header
            title:service.title
        }

        VerticalScrollDecorator {}
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

        clip:true
        anchors.fill:parent
        contentWidth: width;
        contentHeight: fanart.height + header.height + theCol.contentHeight;
        Image {
            id: fanart
            source: service.thumbnail
            height:parent.width / 3
            width:parent.width / 3
            fillMode: Image.PreserveAspectFit
            anchors.right: parent.right
            anchors.top:header.bottom
        }
        Column {
            id:theCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:header.bottom
            spacing:Theme.paddingSmall

            Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                width : main.width - fanart.width - 2 * Theme.horizontalPageMargin
                text:qsTr("<b>Year:</b> %1").arg((service.year !== 0)? service.year:"")
                color:Theme.highlightColor
                verticalAlignment: Text.AlignTop
            }

            Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                width : main.width - fanart.width - 2 * Theme.horizontalPageMargin
                text:qsTr("<b>Genre:</b> %1").arg(service.genres)
                color:Theme.highlightColor
                verticalAlignment: Text.AlignTop
                wrapMode: Text.WordWrap
            }

            Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                width : main.width - fanart.width - 2 * Theme.horizontalPageMargin
                text:qsTr("<b>Rating:</b> %1").arg(Utils.formatRating(service.rating))
                color:Theme.highlightColor
                verticalAlignment: Text.AlignTop
                wrapMode: Text.WordWrap
            }

            Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                width : main.width - fanart.width - 2 * Theme.horizontalPageMargin
                text:qsTr("<b>Runtime:</b> %1").arg(Utils.formatTime(service.runtime))
                color:Theme.highlightColor
                verticalAlignment: Text.AlignTop
                wrapMode: Text.WordWrap
            }

            Button {
                text:qsTr("Play movie")
                onClicked: service.playFile()
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                id:theText
                text:qsTr("Plot:")
                wrapMode: Text.WordWrap
                font.bold: true
                anchors.left: parent.left
                anchors.leftMargin: Theme.horizontalPageMargin
                color:Theme.highlightColor

            }
            Label {
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                text:service.plot
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                color:Theme.highlightColor
            }
        }
    }

    property alias itemId : service.movieId
    property string label
    onItemIdChanged: { service.refresh(); }

    MovieInformationService
    {
        id : service
    }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }

}

