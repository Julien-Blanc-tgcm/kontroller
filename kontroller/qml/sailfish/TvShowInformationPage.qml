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
        anchors.fill: parent
        contentHeight: header.height + col.childrenRect.height
        contentWidth: parent.width
        clip:true

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
            anchors.top:header.bottom
        }

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:header.bottom
            id:col
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
                    anchors.left: col.left
                    anchors.leftMargin: Theme.horizontalPageMargin
                    width:col.width - fanart.width
                    Label {
                        id:txt
                        text: model.modelData.text
                        font.bold: true
                        x:0
                        verticalAlignment: Text.AlignVCenter
                        height:parent.height
                        color:Theme.highlightColor
                    }
                    Label {
                        anchors.left: txt.right
                        anchors.leftMargin: Theme.paddingSmall
                        text:typeof(model.modelData.value) !== "undefined"?model.modelData.value:""
                        verticalAlignment: Text.AlignVCenter
                        height:parent.height
                        width: parent.width - x
                        wrapMode: Text.WordWrap
                        color:Theme.highlightColor
                    }
                }
            }

            Label {
                id:seasonsLbl
                anchors.left: parent.left
                anchors.right: parent.right
                text:qsTr("Seasons:")
                color:Theme.highlightColor
                font.bold: true
                anchors.leftMargin: Theme.horizontalPageMargin
            }

            Repeater {
                id:songs
                model:service.seasons
                delegate: Item {
                    height:Theme.itemSizeSmall
                    anchors.left: col.left
                    anchors.right: col.right
                    anchors.leftMargin: Theme.horizontalPageMargin
                    Label {
                        height: Theme.itemSizeSmall
                        verticalAlignment: Text.AlignVCenter
                        x: Theme.paddingMedium
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
                font.bold: true
                text:qsTr("Plot")
                color:Theme.highlightColor
                anchors.rightMargin: Theme.horizontalPageMargin
                anchors.leftMargin: Theme.horizontalPageMargin
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

