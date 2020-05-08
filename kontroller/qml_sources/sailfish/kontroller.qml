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
    initialPage: Component {
        KontrollerMain {
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
