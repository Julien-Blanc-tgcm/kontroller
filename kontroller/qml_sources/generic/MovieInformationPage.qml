import QtQuick 2.0
import eu.tgcm 1.0
import "utils.js" as Utils
import "."
Item {
    id:main

    function recomputeLayout() {
 /*       if(main.width == 0)
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
        } */
    }

    onWidthChanged: recomputeLayout()
    //onXChanged: if(x===0)recomputeLayout()
    Flickable {

        Text {
            color:"white"
            text:service.title
            font.pixelSize: 16 * scalingFactor
            anchors.left: parent.left
            anchors.top:parent.top
            anchors.right: parent.right
            id:txtTitle
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            z:1
            style:Text.Raised
            styleColor: "#111"
            height:22 * scalingFactor
        }
  /*      Image {
            id:fanart1
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top:parent.top
            source: service.thumbnailservice.thumbnail
            height: {
                if(sourceSize.width != 0)
                    return Math.max(100 * scalingFactor, width * sourceSize.height / sourceSize.width);
                else
                    return txtTitle.contentHeight + 10 * scalingFactor
            }
            fillMode: Image.PreserveAspectCrop
            visible:false;
            onHeightChanged: recomputeLayout()

        } */
        Rectangle {
            height:txtTitle.contentHeight
            width:txtTitle.contentWidth
            x:txtTitle.x + (txtTitle.width - txtTitle.contentWidth) / 2
            y:txtTitle.y + (txtTitle.height - txtTitle.contentHeight) / 2
            visible:false
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
                    height:movieProperties.model.length * 20 * scalingFactor
                    id:propertiesRect
                    color:"black"
                    Repeater {
                        id:movieProperties
                        model:[
                            {text:qsTr("Year:"), value:(service.year !== 0)?service.year:""},
                            {text:qsTr("Genre:"), value:service.genres},
                            {text:qsTr("Rating:"), value:Utils.formatRating(service.rating)},
                            {text:qsTr("Runtime:"), value:Utils.formatTime(service.runtime)}
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
                    font.pixelSize: 12 * scalingFactor
                    height:20 * scalingFactor
                    verticalAlignment: Text.AlignVCenter
                    text:qsTr("Play movie")
                    anchors.top:propertiesRect.bottom
                    anchors.left:parent.left

                    MouseArea {
                        anchors.fill: parent
                        onClicked: service.playFile()
                    }
                }
            }


            Text {
                id:theText
                color:"#eee"
                text:qsTr("Plot:")
                font.pixelSize: 12 * scalingFactor
                wrapMode: Text.WordWrap
                anchors.top: theRect.top
                anchors.topMargin: Math.max(textRect.childrenRect.height, fanart.height) + 5*scalingFactor
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
                anchors.right: parent.right
                anchors.rightMargin: 5 * scalingFactor
                text:service.plot
            }
        }
    }

    property alias itemId : service.movieId
    property string label
    onItemIdChanged: { service.refresh(); }

    MovieInformationService
    {
        id : service
    }
    signal mediaInformationClicked(string filetype, string file, string label)
    function refresh() { service.refresh(); }

}

