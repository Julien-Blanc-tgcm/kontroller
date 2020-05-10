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
        contentHeight: theCol.childrenRect.height + theCol.y
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
            title:service.showTitle
        }
        Column {
            id: theCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: header.bottom
            spacing: Theme.paddingSmall
            PropertiesWithImage {
                anchors.left: parent.left
                anchors.right: parent.right
                imageSource: service.thumbnail
                properties: [
                    qsTr("<b>Episodes:</b> %1").arg(service.nbEpisodes),
                    qsTr("<b>Watched:</b> %1").arg(service.nbWatchedEpisodes)
                ]
            }

            Label {
                id:episodesLbl
                anchors.left: parent.left
                anchors.right: parent.right
                text:qsTr("Episodes:")
                color:Theme.highlightColor
                font.bold: true
                anchors.leftMargin: Theme.horizontalPageMargin
            }

            SilicaListView {
                id:episodes
                model:service.episodes
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: Theme.paddingSmall
                height:childrenRect.height
                delegate: ListItem {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    contentHeight: Math.max(lbl.height, img.height)
                    Image {
                        anchors.left: parent.left
                        anchors.leftMargin: Theme.horizontalPageMargin
                        source: model.modelData.thumbnail
                        id:img
                        fillMode: Image.PreserveAspectFit
                        width: Theme.itemSizeSmall
                        height: width
                        visible: model.modelData.thumbnail.length > 0
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Label {
                        verticalAlignment: Text.AlignVCenter
                        text:model.modelData.label
                        wrapMode: Text.Wrap
                        anchors.left: img.visible? img.right : parent.left
                        anchors.right: btnPlay.left
                        anchors.rightMargin: Theme.paddingSmall
                        anchors.leftMargin: img.visible ? Theme.paddingSmall: Theme.horizontalPageMargin
                        anchors.verticalCenter: parent.verticalCenter
                        id: lbl
                    }
                    IconButton {
                        id: btnPlay
                        icon.source: "image://theme/icon-m-play"
                        anchors.right: parent.right
                        anchors.rightMargin: Theme.horizontalPageMargin
                        onClicked: control.playFile(model.modelData)
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    onClicked: mediaInformationClicked(model.modelData)
                }
            }
        }
    }

    SeasonInformationService {
        id:service
        client: appClient
    }

    VideoControl {
        id:control
        client: appClient
    }

    property alias itemId: service.seasonId
    property string label
    Component.onCompleted: { service.refresh(); }
    signal mediaInformationClicked(var file)
    function refresh() { service.refresh(); }
}

