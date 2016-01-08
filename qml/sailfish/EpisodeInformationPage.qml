import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "utils.js" as Utils
import "."

Page {
    id:main
    SilicaFlickable {
        contentHeight: theCol.childrenRect.height + header.height
        anchors.fill: parent
        clip:true

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

            Repeater {
                id:properties
                model:[
                    {text:qsTr("Rating:"), value:Utils.formatRating(service.rating)},
                    {text:qsTr("Season:"), value:service.season},
                    {text:qsTr("Episode:"), value:service.episode},
                    {text:qsTr("Runtime:"), value:Utils.formatTime(service.runtime)},
                    {text:qsTr("Last played:"), value:Utils.formatDate(service.lastplayed)}
                ]
                delegate: Item {
                    height:Theme.itemSizeSmall
                    anchors.left: theCol.left
                    anchors.right: theCol.right
                    Label {
                        id:txt
                        text: model.modelData.text
                        font.bold: true
                        x:Theme.horizontalPageMargin
                        verticalAlignment: Text.AlignVCenter
                        color:Theme.highlightColor
                    }
                    Label {
                        anchors.left: txt.right
                        anchors.leftMargin: Theme.paddingSmall
                        text:typeof(model.modelData.value) !== "undefined"?model.modelData.value:""
                        color:Theme.highlightColor
                    }
                }
            }

            Button {
                text:qsTr("Play episode")
                id:playlink
                onClicked: service.playFile()
                anchors.horizontalCenter: parent.horizontalCenter
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

    EpisodeInformationService {
        id:service
    }

    property alias itemId: service.episodeId
    property string label
    onItemIdChanged: { service.refresh(); }
    function refresh() { service.refresh(); }
}

