import QtQuick 2.0
import eu.tgcm 1.0
import "."
Item {
    id:main
    Text {
        color:theme.primaryColor
        text:service.name
        font.pixelSize: theme.fontSizeNormal
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
        anchors.topMargin: theme.horizontalPageMargin
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        contentWidth: width;
        contentHeight: col.childrenRect.height;
        interactive: height < contentHeight
        flickableDirection: Flickable.VerticalFlick

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
            id: col
            anchors.top:parent.top

            anchors.left: parent.left
            anchors.right: parent.right
            spacing:theme.paddingSmall

            Text {
                anchors.left: parent.left
                anchors.leftMargin: theme.horizontalPageMargin
                width : main.width - fanart.width - 2 * theme.horizontalPageMargin
                text:qsTr("<b>Artists:</b> %1").arg(service.artists[0])
                color:theme.highlightColor
                verticalAlignment: Text.AlignTop
                font.pixelSize: theme.fontSizeSmall
                wrapMode: Text.WordWrap
            }
            Text {
                anchors.left: parent.left
                anchors.leftMargin: theme.horizontalPageMargin
                width : main.width - fanart.width - 2 * theme.horizontalPageMargin
                text:qsTr("<b>Year:</b> %1").arg((service.year !== 0)? service.year:"")
                color:theme.highlightColor
                verticalAlignment: Text.AlignTop
                font.pixelSize: theme.fontSizeSmall
            }
            Text {
                anchors.left: parent.left
                anchors.leftMargin: theme.horizontalPageMargin
                width : main.width - fanart.width - 2 * theme.horizontalPageMargin
                text:qsTr("<b>Label:</b> %1").arg(service.label)
                color:theme.highlightColor
                verticalAlignment: Text.AlignTop
                font.pixelSize: theme.fontSizeSmall
            }
            Text {
                anchors.left: parent.left
                anchors.leftMargin: theme.horizontalPageMargin
                width : main.width - fanart.width - 2 * theme.horizontalPageMargin
                text:qsTr("<b>Genre:</b> %1").arg(service.genre)
                color:theme.highlightColor
                verticalAlignment: Text.AlignTop
                wrapMode: Text.WordWrap
                font.pixelSize: theme.fontSizeSmall
            }
            Text {
                id : songslbl
                text: qsTr("Songs :")
                font.bold: true
                x:theme.horizontalPageMargin
                color:theme.highlightColor
                font.pixelSize: theme.fontSizeSmall
            }

            Item {
                width:parent.width
                height: service.songs.length * theme.itemSizeSmall
                Repeater {
                    id:songs
                    model:service.songs
                    delegate: Item {
                        height:theme.itemSizeSmall
                        anchors.left: parent.left
                        anchors.right: parent.right
                        y:index * theme.itemSizeSmall
                        Image {
//                            source:"image://theme/icon-m-add"
                            source:"icons/addtoplaylist.png"
                            id:btnaddtopl
                            x:theme.horizontalPageMargin
                            height: theme.itemSizeSmall
                            width:height
                            MouseArea {
                                anchors.fill: parent
                                onClicked: control.addToPlaylist(model.modelData)
                            }

                        }
                        Image {
                            source:"icons/play.png"
                            id:btnplay
                            x:btnaddtopl.width + theme.horizontalPageMargin
                            height:theme.itemSizeSmall
                            width:height
                            MouseArea {
                                anchors.fill: parent
                                onClicked: control.playFile(model.modelData)
                            }
                        }
                        Text {
                            height:theme.itemSizeSmall
                            verticalAlignment: Text.AlignVCenter
                            x:btnplay.x + btnplay.width + theme.paddingSmall
                            text:model.modelData.label
                            clip:true
                            elide: Text.ElideMiddle
                            anchors.left: btnplay.right
                            anchors.leftMargin: theme.horizontalPageMargin
                            anchors.right: parent.right
                            anchors.rightMargin: theme.horizontalPageMargin
                            color:theme.highlightColor
                            font.pixelSize: theme.fontSizeSmall
                        }
                    }
                }
            }

            Text {
                id:theText
                wrapMode: Text.WordWrap
                color:theme.highlightColor
                anchors.left: parent.left
                anchors.leftMargin: theme.horizontalPageMargin
                anchors.right: parent.right
                anchors.rightMargin: theme.horizontalPageMargin
                font.bold: true
                text:qsTr("Description")
                font.pixelSize: theme.fontSizeSmall
            }
            Text {
                color:theme.highlightColor
                font.pixelSize: theme.fontSizeSmall
                wrapMode: Text.WordWrap
                anchors.left: parent.left
                anchors.leftMargin: theme.horizontalPageMargin
                anchors.right: parent.right
                anchors.rightMargin: theme.horizontalPageMargin
                text:service.description
            }
        }
    }

    property alias itemId : service.albumId
    property string label
    onItemIdChanged: service.refresh()

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


