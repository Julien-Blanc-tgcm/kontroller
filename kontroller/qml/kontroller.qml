import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0
import Nemo.Notifications 1.0
import "./components"

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
    property var kontrollerMain: null
    initialPage: Component {
        id: kontrollerMainComponent
        KontrollerMain {
            Component.onCompleted: kontrollerMain = this
        }
    }
    bottomMargin: controllerPanel.visibleSize
    DockedPanel {
        id: controllerPanel
        width: parent.width
        dock: Dock.Bottom
        visible: true
        height: col.height
        open:
        {
            if(pageStack.currentPage && !!pageStack.currentPage.hidePanel)
                return false;
            if(playerControl.player)
                return true;
            return false;
        }

        Column {
            id:col
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.top:parent.top
            spacing: Theme.paddingSmall
            Row {
                visible: !pageStack.currentPage || !pageStack.currentPage.overridePanelImg
                anchors.left: parent.left
                anchors.right: parent.right
                bottomPadding: Theme.paddingSmall
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    Image {
                        source: playerControl.player?playerControl.player.playingInformation.currentItem.thumbnail:""
                        height:Theme.iconSizeLarge
                        width:Theme.iconSizeLarge
                        id:thumbnailImg
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            onClicked: kontrollerMain.pushCurrentlyPlaying()
                        }
                    }
                    Text {
                        text: (appClient && appClient.volumePlugin) ?
                                  "🔈 " + appClient.volumePlugin.formatVolume(appClient.volumePlugin.currentVolume) :
                                  ""
                        width:parent.width
                        font.pixelSize: Theme.fontSizeExtraSmall
                        horizontalAlignment: Text.AlignHCenter
                        color:Theme.primaryColor
                        MouseArea {
                            anchors.fill: parent
                            onClicked: kontrollerMain.pushCurrentlyPlaying()
                        }
                    }
                }

                PlayerControl {
                    anchors.verticalCenter: parent.verticalCenter
                    width:parent.width - thumbnailImg.width
                    player: appClient.playerService.players.length > 0 ? appClient.playerService.players[0] : null
                    id:playerControl
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

}
