import QtQuick 2.0
import eu.tgcm 1.0
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
            txtTitle.width = main.width
        }
        else
        {
            fanart.visible = true;
            fanart.width = fanart.height
            fanart1.visible = false;
            txtTitle.height = 22 * scalingFactor
            txtTitle.width = main.width - fanart.width
            console.log(txtTitle.width + " " + main.width + " " + fanart.width)
        }
    }

    onWidthChanged: recomputeLayout()
    //onXChanged: if(x===0)recomputeLayout()
    Flickable {

        MouseArea {
            anchors.fill: parent
            onClicked: theSubMenu.visible = false
        }

        Text {
            color:"white"
            text:service.artistName
            font.pixelSize: 16 * scalingFactor
            anchors.left: parent.left
            anchors.top:parent.top
            width:parent.width
            id:txtTitle
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            z:1
            style:Text.Raised
            styleColor: "#111"
            height:22 * scalingFactor
        }
        Image {
            id:fanart1
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top:parent.top
            source: service.fanart
            height: {
                if(sourceSize.width != 0)
                    return Math.max(100 * scalingFactor, width * sourceSize.height / sourceSize.width);
                else
                    return txtTitle.contentHeight + 10 * scalingFactor
            }
            fillMode: Image.PreserveAspectCrop
            visible:false;
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
        Image {
            id: fanart
            source: service.thumbnail
            //height:150 * scalingFactor
            height:parent.width / 3
            width:parent.width / 3
            fillMode: Image.PreserveAspectFit
            anchors.right: parent.right
            anchors.top:parent.top
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
        contentHeight: theRect.childrenRect.height + Math.max(txtTitle.height, fanart.height);
        interactive: height < contentHeight
        flickableDirection: Flickable.VerticalFlick
        Rectangle {
            id:theRect
            anchors.top: txtTitle.bottom
            anchors.right: parent.right
            anchors.left: parent.left


            Rectangle {
                id:textRect
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                Text {
                    id:genrelbl
                    anchors.top:parent.top
                    anchors.left: parent.left
                    text: qsTr("Genre :")
                    font.bold: true
                    color:"#eee"
                    font.pixelSize: 12 * scalingFactor
                }
                Text {
                    anchors.top:parent.top
                    anchors.left: genrelbl.right
                    anchors.leftMargin: 5 * scalingFactor
                    text: service.genres
                    color:"#eee"
                    font.pixelSize: 12 * scalingFactor
                }
                Text {
                    id : stylelbl
                    anchors.top:genrelbl.bottom
                    anchors.topMargin: 5 * scalingFactor
                    text: qsTr("Style :")
                    font.bold: true
                    color:"#eee"
                    font.pixelSize: 12 * scalingFactor
                }
                Text {
                    anchors.verticalCenter: stylelbl.verticalCenter
                    anchors.left: stylelbl.right
                    anchors.topMargin: 5 * scalingFactor
                    anchors.leftMargin: 5 * scalingFactor
                    text: service.style
                    color:"#eee"
                    font.pixelSize: 12 * scalingFactor
                }

                Text {
                    id : albumslbl
                    y:Math.max(stylelbl.y + stylelbl.height, fanart.y + fanart.height - contentHeight)
                    //anchors.top: stylelbl.bottom
                    //anchors.topMargin: 5 * scalingFactor
                    text: qsTr("Available albums : ")
                    font.bold: true
                    color: "#eee"
                    font.pixelSize: 12 * scalingFactor
                }

                Rectangle {
                    anchors.top: albumslbl.bottom
                    anchors.topMargin: 5 * scalingFactor
                    anchors.left: parent.left
                    height:20 * touchScalingFactor * service.albums.length
                    id:rect
                    Repeater {
                        id:albums
                        model:service.albums
                        delegate: Rectangle {
                            height: 20 * touchScalingFactor
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
                                id:albumtext
                                color:"#eee"
                                font.pixelSize: 12 * scalingFactor
                                height: 20 * touchScalingFactor
                                verticalAlignment: Text.AlignVCenter
                                x:btnplay.x + btnplay.width+ 5 * scalingFactor
                                y:index * 20 * touchScalingFactor
                                text:model.modelData.label
                                clip:true
                                elide: Text.ElideMiddle
                                width:theRect.width - btnplay.width  - btnaddtopl.width - 10 * scalingFactor
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: mediaInformationClicked(model.modelData.filetype,
                                                                       model.modelData.file,
                                                                       model.modelData.label)
                                    onPressAndHold: {
                                        theSubMenu.showSubMenu(rect, model.modelData, mouseX, mouseY)
                                    }
                                }
                            }

                        }
                    }
                }

            }
            Text {
                id:lblDesc
                font.bold: true
                anchors.top: theRect.top
                anchors.topMargin: Math.max(textRect.childrenRect.height, fanart.height)
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
                color: Styling.textColor
                font.pixelSize: 12 * scalingFactor
                text: qsTr("Description")
            }

            Text {
                id:theText
                color:"#eee"
                text:service.artistDescription
                font.pixelSize: 12 * scalingFactor
                wrapMode: Text.WordWrap
                anchors.top: lblDesc.bottom
                anchors.topMargin:5 * scalingFactor
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
            }
        }
    }

    property alias itemId : service.artistId
    property string label
    onItemIdChanged: { service.refresh(); }
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
    MusicControl {
        id:control
    }

    ArtistInformationService
    {
        id : service
    }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }
}

