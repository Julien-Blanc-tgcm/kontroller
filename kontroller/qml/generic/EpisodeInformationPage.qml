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
                        {text:qsTr("Rating:"), value:Utils.formatRating(service.rating)},
                        {text:qsTr("Season:"), value:service.season},
                        {text:qsTr("Episode:"), value:service.episode},
                        {text:qsTr("Runtime:"), value:Utils.formatTime(service.runtime)},
                        {text:qsTr("Last played:"), value:Utils.formatDate(service.lastplayed)}
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

            Text {
                color:Styling.linkColor
                text:qsTr("Play episode")
                font.pixelSize: 12 * scalingFactor
                anchors.top:propertiesRect.bottom
                anchors.topMargin: 10 * scalingFactor
                anchors.left: parent.left
                id:playlink
                MouseArea {
                    anchors.fill: parent
                    onClicked: service.playFile()
                }
            }

            Text {
                id:theText
                color:"#eee"
                text:qsTr("Plot:")
                font.pixelSize: 12 * scalingFactor
                anchors.top: playlink.bottom
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

    EpisodeInformationService {
        id:service
    }

    property alias itemId: service.episodeId
    property string label
    onItemIdChanged: { service.refresh(); }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }
}

