import QtQuick 2.0
import harbour.eu.tgcm 1.0
import "utils.js" as Utils
import "."

Item {
    id:main
    function recomputeLayout() {
        if(main.width == 0)
            return;
        if(main.width / scalingFactor < 200)
        {
            fanart.visible = false;
            fanart1.visible = true;
            txtTitle.height = fanart1.height
            fanart.width = 0;
        }
        else
        {
            fanart.visible = true;
            fanart.width = fanart.height
            fanart1.visible = false;
            txtTitle.height = 22 * scalingFactor
        }
    }
    onWidthChanged: recomputeLayout()
    Flickable {

        MouseArea {
            anchors.fill: parent
            onClicked: theSubMenu.visible = false;
        }

        Text {
            color:"white"
            text:service.showTitle
            font.pixelSize: 16 * scalingFactor
            anchors.left: parent.left
            anchors.top:parent.top
            anchors.right: parent.right
            id:txtTitle
            wrapMode:Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            z:1
            style:Text.Raised
            styleColor: "#111"
            height:22*scalingFactor
        }
        Image {
            id:fanart1
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top:parent.top
            source: service.art.length > 0?service.art:service.fanart
            height : {
                if(sourceSize.width != 0)
                    return Math.max(100 * scalingFactor, width * sourceSize.height / sourceSize.width)
                else
                    return txtTitle.contentHeight + 10 * scalingFactor
            }
            fillMode: Image.PreserveAspectCrop
            visible:false
            onHeightChanged: recomputeLayout()
        }
        Rectangle {
            height:txtTitle.contentHeight
            width:txtTitle.contentWidth
            x:txtTitle.x + (txtTitle.width - txtTitle.contentWidth) / 2
            y:txtTitle.y + (txtTitle.height - txtTitle.contentHeight) / 2
            visible:fanart1.visible
            color:"black"
            opacity: 0.2
        }

        clip:true
        anchors.top : parent.top
        anchors.topMargin: 5 * scalingFactor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 5 * scalingFactor
        anchors.rightMargin: 5 * scalingFactor
        contentWidth: width;
        contentHeight: theRect.childrenRect.height + txtTitle.height;
        interactive: height < contentHeight
        flickableDirection: Flickable.VerticalFlick

        Rectangle {
            id:theRect
            anchors.top: txtTitle.bottom
            anchors.right: parent.right
            anchors.left: parent.left

            Image {
                id: fanart
                source: service.thumbnail //(service.thumbnail.length > 0)?("image://kodi/" + service.thumbnail):""
                //height:150 * scalingFactor
                height:parent.width / 3
                width:parent.width / 3
                fillMode: Image.PreserveAspectFit
                anchors.right: parent.right
                anchors.top:parent.top
            }
            Item {
                id:textRect
                anchors.left: parent.left
                anchors.right: fanart.left

                Rectangle {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.left: parent.left
                    height:properties.model.length * 20 * scalingFactor
                    id:propertiesRect
                    color:"black"
                    Repeater {
                        id:properties
                        model:[
                            {text:qsTr("Episodes:"), value:service.nbEpisodes},
                            {text:qsTr("Watched:"), value:service.nbWatchedEpisodes}
                        ]
                        delegate: Item {

                            Text {
                                id:txt
                                height:20 * scalingFactor
                                text: model.modelData.text
                                font.bold: true
                                color:"#eee"
                                font.pixelSize: 12 * scalingFactor
                                x:0
                                y:index * 20 * scalingFactor
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                anchors.verticalCenter: txt.verticalCenter
                                anchors.left: txt.right
                                anchors.leftMargin: 5 * scalingFactor;
                                color:"#eee"
                                font.pixelSize: 12 * scalingFactor
                                text:typeof(model.modelData.value) !== "undefined"?model.modelData.value:""
                            }
                        }
                    }
                }
            }
            Text {
                id:episodesLbl
                anchors.top:theRect.top
                anchors.topMargin: propertiesRect.height + 10 * scalingFactor
                anchors.left: parent.left
                anchors.right: parent.right
                height : 20 * touchScalingFactor
                text:qsTr("Episodes:")
                font.pixelSize: 12 * scalingFactor
                color:"white"
            }

            Rectangle {
                anchors.top:episodesLbl.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height : itemHeight * service.episodes.length
                property int itemHeight : 20 * touchScalingFactor
                id:episodesList
                color:"black"
                Repeater {
                    id:songs
                    model:service.episodes
                    delegate: Rectangle {
                        height:episodesList.itemHeight
                        Image {
                            source: "icons/play.png"
                            width:height
                            height:18 * touchScalingFactor
                            x:0
                            y:index * 20 * touchScalingFactor
                            MouseArea {
                                anchors.fill: parent
                                onClicked: control.playFile(model.modelData)
                            }
                        }

                        Text {
                            color:Styling.linkColor
                            font.pixelSize: 12 * scalingFactor
                            height: 20 * touchScalingFactor
                            verticalAlignment: Text.AlignVCenter
                            x: 25 * scalingFactor
                            y:index * 20 * touchScalingFactor
                            text:model.modelData.label
                            clip:true
                            elide: Text.ElideMiddle
                            width:theRect.width - 25 * scalingFactor
                            MouseArea {
                                anchors.fill: parent
                                onClicked: mediaInformationClicked(model.modelData.filetype,
                                                                       model.modelData.file,
                                                                       model.modelData.label)
                                onPressAndHold: {
                                    theSubMenu.showSubMenu(parent, model.modelData, mouseX, mouseY)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ListContextMenu {
        visible:false
        color:"black"
        border.color: Styling.borderColor
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
    }

    SeasonInformationService {
        id:service
    }

    VideoControl {
        id:control
    }

    property alias itemId: service.seasonId
    property string label
    onItemIdChanged: { service.refresh(); }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }
}

