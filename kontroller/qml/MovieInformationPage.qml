import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "utils.js" as Utils
import "."
import "./components"

Page {
    id:main
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    SilicaFlickable {
        visible: !service.refreshing

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
        contentHeight: header.height + theCol.childrenRect.height + Theme.paddingMedium
        Column {
            id: theCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:header.bottom
            spacing:Theme.paddingSmall

            PropertiesWithImage {
                id: fanart
                imageSource: service.thumbnail
                anchors.left: parent.left
                anchors.right: parent.right
                properties: [
                    qsTr("<b>Year:</b> %1").arg((service.year !== 0)? service.year:""),
                    qsTr("<b>Genre:</b> %1").arg(service.genres),
                    qsTr("<b>Rating:</b> %1").arg(Utils.formatRating(service.rating)),
                    qsTr("<b>Runtime:</b> %1").arg(Utils.formatTime(service.runtime))
                ]
            }

            Column { // create a new column to avoid spacing between the two elements
                width:parent.width
                ResumeButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    onResume: service.resumeFile()
                    onPlay: service.playFile()
                    labelPlay: qsTr("Play movie")
                    labelResume: qsTr("Resume movie")
                    offerResume: service.resumePosition !== 0
                }
                Label {
                    text: qsTr("(at %1)").arg(Utils.formatTime(service.resumePosition))
                    font.italic: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Theme.highlightColor
                    visible: service.resumePosition !== 0
                }
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
    BusyIndicator {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        visible:service.refreshing
        running: visible
        size: BusyIndicatorSize.Large
    }

    property alias itemId : service.movieId
    property string label
    Component.onCompleted: {
        service.refresh();
    }

    MovieInformationService
    {
        id : service
        client: appClient
    }
    signal mediaInformationClicked(var file)
    function refresh() { service.refresh(); }

}

