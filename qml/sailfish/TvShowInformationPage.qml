import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "utils.js" as Utils
import "."

Page {
    id:main
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: txtTitle.height + theCol.height
        contentWidth: parent.width
        clip:true

        PageHeader {
            id:header
            title:service.title
        }

        Image {
            id: fanart
            source: service.thumbnail
            height:parent.width / 3
            width:parent.width / 3
            fillMode: Image.PreserveAspectFit
            anchors.right: parent.right
            anchors.top:parent.top
        }

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:header.bottom
            Repeater {
                id:properties
                model:[
                    {text:qsTr("Year:"), value:(service.year !== 0)?service.year:""},
                    {text:qsTr("Genre:"), value:Utils.join(service.genres, ',')},
                    {text:qsTr("Rating:"), value:Utils.formatRating(service.rating)},
                    {text:qsTr("Seasons:"), value:service.nbSeasons},
                    {text:qsTr("Episodes:"), value:service.nbEpisodes}
                ]
                delegate: Item {
                    height:Theme.itemSizeSmall
                    anchors.left: parent.left
                    anchors.right: parent.right
                    Label {
                        id:txt
                        text: model.modelData.text
                        font.bold: true
                        x:0
                        verticalAlignment: Text.AlignVCenter
                        height:parent.height
                    }
                    Label {
                        anchors.left: txt.right
                        anchors.leftMargin: 5
                        text:typeof(model.modelData.value) !== "undefined"?model.modelData.value:""
                        verticalAlignment: Text.AlignVCenter
                        height:parent.height
                    }
                }
            }

            Label {
                id:seasonsLbl
                anchors.left: parent.left
                anchors.right: parent.right
                text:qsTr("Seasons:")
            }

            Repeater {
                id:songs
                model:service.seasons
                delegate: Rectangle {
                    height:Theme.itemSizeSmall
                    Label {
                        height: Theme.itemSizeSmall
                        verticalAlignment: Text.AlignVCenter
                        x: 10
                        text:model.modelData.label
                        clip:true
                        elide: Text.ElideMiddle
                        width:main.width - 15
                        MouseArea {
                            anchors.fill: parent
                            onClicked: mediaInformationClicked(model.modelData.filetype,
                                                                   model.modelData.file,
                                                                   model.modelData.label)
                            onPressAndHold: {
                             //   theSubMenu.showSubMenu(parent, model.modelData, mouseX, mouseY)
                            }
                        }
                    }
                }
            }

            Label {
                id:theText
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                font.bold: true
            }
            Label {
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.WordWrap
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5
                text:service.plot
                textFormat: Text.PlainText
            }
        }
    }

   /* ListContextMenu {
        visible:false
        color:"black"
        border.width: 2 * scalingFactor
        id:theSubMenu
        onInformationPressed:
        {
            visible = false
            if(currentModel)
                mediaInformationClicked(currentModel.filetype, currentModel.file, currentModel.label)
        }
        onAddToPlaylistPressed:  {
            visible = false
            control.addToPlaylist(currentModel)
        }
        onPlayPressed: {
            visible = false
            control.playFile(currentModel)
        }
    } */

    TvShowInformationService {
        id:service
    }

    VideoControl {
        id:control
    }

    property alias itemId: service.tvshowId
    property string label
    onItemIdChanged: { service.refresh(); }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }
}

