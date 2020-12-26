import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "utils.js" as Utils
import "."
import "./components"

Page {
    id:main
    SilicaFlickable {
        contentHeight: theCol.childrenRect.height + header.height
        anchors.fill: parent
        clip:true
        visible: !service.refreshing

        VerticalScrollDecorator {}
        PageHeader {
            id:header
            title:service.title
            wrapMode:Text.WordWrap
        }

        Image {
            id: fanart
            source: service.thumbnail
            height:parent.width / 3
            width:parent.width / 3
            fillMode: Image.PreserveAspectFit
            anchors.right: parent.right
            anchors.top:header.bottom
            anchors.rightMargin: Theme.horizontalPageMargin
        }

        Column {
            id:theCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: header.bottom
            spacing: Theme.paddingSmall

            PropertiesWithImage {
                anchors.left: parent.left
                anchors.right: parent.right
                properties: [
                    qsTr("<b>Rating:</b> %1").arg(Utils.formatRating(service.rating)),
                    qsTr("<b>Season:</b> %1").arg(service.season),
                    qsTr("<b>Episode:</b> %1").arg(service.episode),
                    qsTr("<b>Runtime:</b> %1").arg(Utils.formatTime(service.runtime)),
                    qsTr("<b>Last played:</b> %1").arg(Utils.formatDate(service.lastplayed))
                ]
            }

            Column { // create a new column to avoid spacing between the two elements
                width:parent.width
                ResumeButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    onResume: service.resumeFile()
                    onPlay: service.playFile()
                    labelPlay: qsTr("Play episode")
                    labelResume: qsTr("Resume episode")
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
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
                anchors.leftMargin: Theme.horizontalPageMargin
                font.bold: true
                color:Theme.highlightColor
            }
            Label {
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.WordWrap
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: Theme.horizontalPageMargin
                anchors.leftMargin: Theme.horizontalPageMargin
                text:service.plot
                textFormat: Text.PlainText
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

    EpisodeInformationService {
        id:service
        client: appClient
    }

    property alias itemId: service.episodeId
    property string label
    Component.onCompleted: { refresh(); }
    function refresh() { service.refresh(); }
}

