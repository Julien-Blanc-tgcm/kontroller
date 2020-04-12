import QtQuick 2.0
import eu.tgcm 1.0
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
            text:service.title
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
            source: (service.art.length > 0)?service.art:service.fanart;
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
                source: service.thumbnail
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
                            {text:qsTr("Year:"), value:(service.year !== 0)?service.year:""},
                            {text:qsTr("Genre:"), value:Utils.join(service.genres, ',')},
                            {text:qsTr("Rating:"), value:Utils.formatRating(service.rating)},
                            {text:qsTr("Seasons:"), value:service.nbSeasons},
                            {text:qsTr("Episodes:"), value:service.nbEpisodes}
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
                id:seasonsLbl
                anchors.top:theRect.top
                anchors.topMargin: propertiesRect.height + 10 * scalingFactor
                anchors.left: parent.left
                anchors.right: parent.right
                height : 20 * touchScalingFactor
                text:qsTr("Seasons:")
                font.pixelSize: 12 * scalingFactor
                color:"white"
            }

            Rectangle {
                anchors.top:seasonsLbl.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height : itemHeight * service.seasons.length
                property int itemHeight : 20 * touchScalingFactor
                id:seasonsList
                color:"black"
                Repeater {
                    id:songs
                    model:service.seasons
                    delegate: Rectangle {
                        height:seasonsList.itemHeight

                        Text {
                            color:Styling.linkColor
                            font.pixelSize: 12 * scalingFactor
                            height: 20 * touchScalingFactor
                            verticalAlignment: Text.AlignVCenter
                            x: 10 * scalingFactor
                            y:index * 20 * touchScalingFactor
                            text:model.modelData.label
                            clip:true
                            elide: Text.ElideMiddle
                            width:theRect.width - 15 * scalingFactor
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


            Text {
                id:theText
                color:"#eee"
                text:qsTr("Plot:")
                font.pixelSize: 12 * scalingFactor
                anchors.top: seasonsList.bottom
                anchors.topMargin: 10 * scalingFactor
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
                font.bold: true
            }
            Text {
                color:"#eee"
                font.pixelSize: 12 * scalingFactor
                wrapMode: Text.WordWrap
                anchors.top:theText.bottom
                anchors.topMargin: 5 * scalingFactor
                anchors.left: parent.left
                anchors.leftMargin: 5 * scalingFactor
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
                text:service.plot
                textFormat: Text.PlainText
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

    TvShowInformationService {
        id:service
        client: appClient
    }

    VideoControl {
        id:control
        client: appClient
    }

    property alias itemId: service.tvshowId
    property string label
    onItemIdChanged: { service.refresh(); }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }
}

