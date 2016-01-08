import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "utils.js" as Utils
import "."

Page {
    id:main
    SilicaFlickable {
        contentHeight: theCol.childrenRect.height
        anchors.fill: parent
        clip:true

        VerticalScrollDecorator {}

        Column {
            id:theCol
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
            }
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
                    anchors.left: parent.left
                    anchors.right: parent.right
                    Text {
                        id:txt
                        text: model.modelData.text
                        font.bold: true
                        x:0
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        anchors.left: txt.right
                        anchors.leftMargin: 5
                        text:typeof(model.modelData.value) !== "undefined"?model.modelData.value:""
                    }
                }
            }

            Button {
                text:qsTr("Play episode")
                anchors.left: parent.left
                id:playlink
                onClicked: service.playFile()
            }

            Label {
                id:theText
                text:qsTr("Plot:")
                anchors.left: parent.left
                anchors.right: parent.right
                font.bold: true
            }
            Label {
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.WordWrap
                anchors.left: parent.left
                anchors.right: parent.right
                text:service.plot
                textFormat: Text.PlainText
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

