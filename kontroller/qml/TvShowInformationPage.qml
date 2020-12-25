import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "utils.js" as Utils
import "."
import "./components"

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

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:header.bottom
            spacing: Theme.paddingSmall
            id:col
            PropertiesWithImage {
                anchors.left: parent.left
                anchors.right: parent.right
                properties: [
                    qsTr("<b>Year:</b> %1").arg((service.year !== 0)?service.year:""),
                    qsTr("<b>Genre:</b> %1").arg(Utils.join(service.genres, ',')),
                    qsTr("<b>Rating:</b> %1").arg(Utils.formatRating(service.rating)),
                    qsTr("<b>Seasons:</b> %1").arg(service.nbSeasons),
                    qsTr("<b>Episodes:</b> %1").arg(service.nbEpisodes)
                ]
                imageSource: service.thumbnail
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

            SilicaListView {
                id:seasons
                model:service.seasons
                height: childrenRect.height
                anchors.left:parent.left
                anchors.right: parent.right
                spacing: Theme.paddingSmall
                delegate: ListItem {
                    contentHeight:Math.max(seasonLabel.height, seasonImg.height)
                    anchors.left: parent.left
                    anchors.right: parent.right
                    Image {
                        anchors.left: parent.left
                        anchors.leftMargin: Theme.horizontalPageMargin
                        visible:model.modelData.thumbnail.length > 0
                        source:model.modelData.thumbnail
                        height: Theme.itemSizeSmall
                        width: height
                        anchors.verticalCenter: parent.verticalCenter
                        id:seasonImg
                        fillMode: Image.PreserveAspectFit
                    }
                    Label {
                        id:seasonLabel
                        height: Theme.itemSizeSmall
                        verticalAlignment: Text.AlignVCenter
                        anchors.left: seasonImg.visible?seasonImg.right:parent.left
                        anchors.leftMargin: seasonImg.visible?Theme.paddingSmall:Theme.horizontalPageMargin
                        anchors.verticalCenter: parent.verticalCenter
                        text:model.modelData.label
                        clip:true
                        wrapMode: Text.Wrap
                    }
                    onClicked: mediaInformationClicked(model.modelData, {"tvShowId":itemId})
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
    Component.onCompleted: {
        service.refresh();
    }
    signal mediaInformationClicked(var file, var extraData)
    function refresh() { service.refresh(); }
}

