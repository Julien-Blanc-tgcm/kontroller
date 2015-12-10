import QtQuick 2.0

import QtQuick 2.0
import eu.tgcm 1.0

Item {
    id:main
    Text {
        color:"white"
        text:service.name
        font.pixelSize: 16 * scalingFactor
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalAlignment: Text.AlignHCenter
        anchors.top:parent.top
        id:txtTitle
        wrapMode: Text.WordWrap
        clip:true
    }

    Flickable {
        clip:true
        anchors.top : txtTitle.bottom
        anchors.topMargin: 5 * scalingFactor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 5 * scalingFactor
        anchors.rightMargin: 5 * scalingFactor
        contentWidth: width;
        contentHeight: theRect.childrenRect.height;
        interactive: height < contentHeight
        flickableDirection: Flickable.VerticalFlick
        MouseArea {
            anchors.fill: parent
            onClicked: theSubMenu.visible = false;
        }

        Rectangle {
            id:theRect
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left

            Image {
                id: fanart
                source: (service.thumbnail.length > 0)?("image://kodi/" + service.thumbnail):""
                //height:150 * scalingFactor
                height:parent.width / 3
                width:parent.width / 3
                fillMode: Image.PreserveAspectFit
                anchors.right: parent.right
                anchors.top:parent.top
            }
            Rectangle {
                id:textRect
                Rectangle {
                    anchors.top: textRect.top
                    anchors.topMargin: 5 * scalingFactor
                    anchors.left: parent.left
                    height:songProperties.model.length * 20 * scalingFactor
                    id:propertiesRect
                    Repeater {
                        id:songProperties
                        model:[
                            {text:qsTr("Artists:"), value:service.artists[0]},
                            {text:qsTr("Year:"), value:(service.year !== 0)?service.year:""},
                            {text:qsTr("Label:"), value:service.label},
                            {text:qsTr("Genre:"), value:service.genre}
                        ]
                        delegate: Item{

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

                Text {
                    id : songslbl
                    anchors.top: textRect.top
                    anchors.topMargin: Math.max(fanart.height, propertiesRect.height) +  5 * scalingFactor
                    text: qsTr("Songs :")
                    font.bold: true
                    color: "#eee"
                    font.pixelSize: 12 * scalingFactor
                }
                Rectangle {
                    anchors.top: songslbl.bottom
                    anchors.topMargin: 5 * scalingFactor
                    anchors.left: parent.left
                    anchors.leftMargin: 10 * scalingFactor
                    height:20 * touchScalingFactor * service.songs.length
                    Repeater {
                        id:songs
                        model:service.songs
                        delegate: Rectangle {
                            height:20 * touchScalingFactor
                            Image {
                                width: height
                                height: 18 * touchScalingFactor
                                source:"icons/addtoplaylist.png"
                                id:btnaddtopl
                                x:0
                                y:index * 20 * touchScalingFactor
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: control.addToPlaylist(model.modelData)
                                }
                            }
                            Image {
                                width: height
                                height: 18 * touchScalingFactor
                                source:"icons/play.png"
                                id:btnplay
                                x:width + 5 * scalingFactor
                                y:index * 20 * touchScalingFactor
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: control.playFile(model.modelData)
                                }
                            }
                            Text {
                                color:"#eee"
                                font.pixelSize: 12 * scalingFactor
                                height: 20 * touchScalingFactor
                                x:btnplay.x + btnplay.width+ 5 * scalingFactor
                                y:index * 20 * touchScalingFactor
                                text:model.modelData.label
                                clip:true
                                elide: Text.ElideMiddle
                                width:theRect.width - btnplay.width  - btnaddtopl.width - 15 * scalingFactor
                                MouseArea {
                                    anchors.fill: parent
                                    /*onClicked: mediaInformationClicked(model.modelData.filetype,
                                                                       model.modelData.file,
                                                                       model.modelData.label) */
                                    onPressAndHold: {
                                        theSubMenu.showSubMenu(parent, model.modelData, mouseX, mouseY)
                                    }
                                }
                            }
                        }
                    }

                }

            }

            Text {
                id:theText
                color:"#eee"
                font.pixelSize: 12 * scalingFactor
                wrapMode: Text.WordWrap
                anchors.top: theRect.top
                anchors.topMargin: Math.max(textRect.childrenRect.height, fanart.height) + 5 * scalingFactor
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
                font.bold: true
                text:qsTr("Description")
            }
            Text {
                color:"#eee"
                font.pixelSize: 12 * scalingFactor
                wrapMode: Text.WordWrap
                anchors.top: theText.bottom
                anchors.topMargin: 5 * scalingFactor
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
                text:service.description
            }
        }
    }

    property alias itemId : service.albumId
    property string label
    onItemIdChanged: service.refresh()

    ListContextMenu {
        visible:false
        color:"black"
        border.color: appstyle.borderColor
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

    AlbumInformationService
    {
        id : service
    }
    MusicControl
    {
        id: control
    }

    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }
}


