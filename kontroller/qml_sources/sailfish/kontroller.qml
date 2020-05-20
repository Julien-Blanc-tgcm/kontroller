import QtQuick 2.2
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0
import Nemo.Notifications 1.0

ApplicationWindow {

   // color: "#000"

//    property alias scalingFactor : deviceInformation.scalingFactor
//    property alias touchScalingFactor: deviceInformation.touchScalingFactor

    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All
    cover: {
        if(appClient.connectionStatus === 2)
            return Qt.resolvedUrl("cover/CoverPage.qml")
        else
            return Qt.resolvedUrl("cover/UnconnectedCover.qml");
    }
    id: appWindow
    initialPage: Component {
        KontrollerMain {
        }
    }
    bottomMargin: controllerPanel.visibleSize
    DockedPanel {
        id: controllerPanel
        width: parent.width
        dock: Dock.Bottom
        visible: true
        height: col.height
        contentHeight: height
        flickableDirection: Flickable.VerticalFlick
        z:1
        open: false

        Column {
            id:col
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.top:parent.top
            spacing: Theme.paddingSmall
            Row {
                anchors.left: parent.left
                anchors.right: parent.right
                visible: playerControl.player
                Image {
                    source: playerControl.player?playerControl.player.playingInformation.currentItem.thumbnail:""
                    height:Theme.iconSizeLarge
                    width:Theme.iconSizeLarge
                    id:thumbnailImg
                    fillMode: Image.PreserveAspectFit
                    z: 2
                    anchors.verticalCenter: parent.verticalCenter
                }
                PlayerControl {
                    anchors.verticalCenter: parent.verticalCenter
                    width:parent.width - thumbnailImg.width
                    player: appClient.playerService.players.length > 0 ? appClient.playerService.players[0] : null
                    id:playerControl
                }
            }
            VolumeControl {
                anchors.left: parent.left
                anchors.right: parent.right
                volumePlugin: appClient?appClient.volumePlugin:null
                visible: (!playerControl.player) || (updating && !pushUpMenu.active)
                id: volumeControl
            }
        }
        PushUpMenu {
            bottomMargin: Theme.paddingSmall
            id: pushUpMenu
            parent: controllerPanel
            visible: true
            Column {
                //visible: playerControl.player && playerControl.player.type === "video"
                anchors.left: parent.left
                anchors.right: parent.right
                id:theColumn
                VolumeControl {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    volumePlugin: appClient?appClient.volumePlugin:null
                    visible: true
                }
                PlayerProperties {
                    player: playerControl.player
                    id: playerProperties
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }
        }
    }

    StatusService {
        id:statusService
        settings: appSettings
    }

    Notification {
        id: theNotif
        appName: "kontroller"
        appIcon: "/usr/share/icons/hicolor/86x86/apps/kontroller.png"
        category: "kontroller"
    }

    Connections {
        target: appClient.downloadService
        onDownloadStarted:
        {
            var theFile = file;
            var message = qsTr("Download of %1 started").arg(theFile);
            theNotif.body = message
            theNotif.previewBody = message;
            theNotif.summary = qsTr("Download started");
            theNotif.previewSummary = qsTr("Download started");
            theNotif.publish()
        }
        onDownloadCompleted: {
            var theFile = file;
            var message = qsTr("Download of %1 complete").arg(theFile);
            theNotif.body = message
            theNotif.previewBody = message;
            theNotif.summary = qsTr("Download complete");
            theNotif.previewSummary = qsTr("Download complete");
            theNotif.publish()
        }
        onDownloadError: {
            var theFile = file;
            var message = qsTr("Download of %1 failed with error %2").arg(theFile).arg(error);
            theNotif.body = message
            theNotif.previewBody = message
            theNotif.summary = qsTr("Download error");
            theNotif.previewSummary = qsTr("Download error");
            theNotif.publish()
        }
    }

    function recomputeDockVisible()
    {
        controllerPanel.open = playerControl.player || volumeControl.updated || volumeControl.updating
    }

    Connections {
        target: playerControl
        onPlayerChanged: recomputeDockVisible()
    }

    Connections {
        target: volumeControl
        onUpdatedChanged: recomputeDockVisible()
        onUpdatingChanged: recomputeDockVisible()
    }

}
