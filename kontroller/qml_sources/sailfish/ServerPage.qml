import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0
/*
  Server Page, containing actions related to the server. ≠ from Server Settings Page
  */
Page {
    id: root
    property var remorse: nullptr
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: theCol.height

        PullDownMenu {
            visible: true
            MenuItem {
                text: qsTr("Shutdown server")
                onClicked: remorse = Remorse.popupAction(root,
                                                         text,
                                                         function() { systemService.shutdownServer(); })
            }
            MenuItem {
                text: qsTr("Reboot server")
                onClicked: remorse = Remorse.popupAction(root,
                                                         text,
                                                         function() { systemService.rebootServer(); })

            }
        }

        Column {
            id: theCol
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.rightMargin: Theme.horizontalPageMargin
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Server management")
            }

            SectionHeader {
                text: qsTr("Video library")
            }

            Label {
                text: qsTr("Removes item that have been deleted on disk from the library.")
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignLeft
                width:parent.width
                color: Theme.highlightColor
            }
            Button {
                id: btnCleanVideoLibrary
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Clean library")
                onClicked: service.cleanVideoLibrary()
            }
            BusyIndicator {
                id: busyCleanVideoLibrary
                visible: false
                running: false
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Label {
                id: finishedCleanVideoLibrary
                text: qsTr("Video library cleaning finished!");
                visible: false
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.italic: true
                color: Theme.highlightColor
            }

            Label {
                text: qsTr("Search for new items in the video library.")
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignLeft
                width:parent.width
                color: Theme.highlightColor
            }

            Button {
                id: btnRefreshVideoLibrary
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Refresh library")
                onClicked: service.refreshVideoLibrary()
            }
            BusyIndicator {
                id: busyRefreshVideoLibrary
                visible: false
                running: false
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Label {
                id: finishedRefreshVideoLibrary
                text: qsTr("Video library refresh finished!");
                visible: false
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.italic: true
                color: Theme.highlightColor
            }



            SectionHeader {
                text: qsTr("Audio library")
            }

            Label {
                text: qsTr("Removes item that have been deleted on disk from the library.")
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignLeft
                width:parent.width
                color: Theme.highlightColor
            }
            Button {
                id: btnCleanAudioLibrary
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Clean library")
                onClicked: service.cleanAudioLibrary()
            }
            BusyIndicator {
                id: busyCleanAudioLibrary
                visible: false
                running: false
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Label {
                id: finishedCleanAudioLibrary
                text: qsTr("Audio library cleaning finished!");
                visible: false
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.italic: true
                color: Theme.highlightColor
            }


            // scan audio library
            Label {
                text: qsTr("Search for new items in the video library.")
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignLeft
                width:parent.width
                color: Theme.highlightColor
            }
            Button {
                id: btnRefreshAudioLibrary
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Refresh library")
                onClicked: service.refreshAudioLibrary()
            }
            BusyIndicator {
                id: busyRefreshAudioLibrary
                visible: false
                running: false
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Label {
                id: finishedRefreshAudioLibrary
                text: qsTr("Audio library refresh finished!");
                visible: false
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.italic: true
                color: Theme.highlightColor
            }
        }
    }

    Timer {
        id: cleanVideoTimer
        interval: 1500
        repeat: false
        running: false
        onTriggered: {
            btnCleanVideoLibrary.visible = true;
            finishedCleanVideoLibrary.visible = false;
        }
    }
    Timer {
        id: cleanAudioTimer
        interval: 1500
        repeat: false
        running: false
        onTriggered: {
            btnCleanAudioLibrary.visible = true;
            finishedCleanAudioLibrary.visible = false;
        }
    }
    Timer {
        id: refreshAudioTimer
        interval: 1500
        repeat: false
        running: false
        onTriggered: {
            btnRefreshAudioLibrary.visible = true;
            finishedRefreshAudioLibrary.visible = false;
        }
    }
    Timer {
        id: refreshVideoTimer
        interval: 1500
        repeat: false
        running: false
        onTriggered: {
            btnRefreshVideoLibrary.visible = true;
            finishedRefreshVideoLibrary.visible = false;
        }
    }
    LibraryService {
        id: service
        client: appClient
        onAudioLibraryCleanProgressChanged: {
            console.log("audio clean progress " + service.audioLibraryCleanProgress)
            if(service.audioLibraryCleanProgress == 0)
            {
                busyCleanAudioLibrary.visible = false;
                btnCleanAudioLibrary.visible = true;
                finishedCleanAudioLibrary.visible = false;
            }
            else if(service.audioLibraryCleanProgress == 100)
            {
                busyCleanAudioLibrary.visible = false;
                btnCleanAudioLibrary.visible = false;
                finishedCleanAudioLibrary.visible = true;
                cleanAudioTimer.start()
            }
            else
            {
                busyCleanAudioLibrary.visible = true;
                busyCleanAudioLibrary.running = true;
                btnCleanAudioLibrary.visible = false;
                finishedCleanAudioLibrary.visible = false;
            }
        }
        onVideoLibraryCleanProgressChanged: {
            console.log("video clean progress " + service.videoLibraryCleanProgress)
            if(service.videoLibraryCleanProgress == 0)
            {
                busyCleanVideoLibrary.visible = false;
                btnCleanVideoLibrary.visible = true;
                finishedCleanVideoLibrary.visible = false;
            }
            else if(service.videoLibraryCleanProgress == 100)
            {
                busyCleanVideoLibrary.visible = false;
                btnCleanVideoLibrary.visible = false;
                finishedCleanVideoLibrary.visible = true;
                cleanVideoTimer.start()
            }
            else
            {
                busyCleanVideoLibrary.visible = true;
                busyCleanVideoLibrary.running = true;
                btnCleanVideoLibrary.visible = false;
                finishedCleanVideoLibrary.visible = false;
            }
        }
        onAudioLibraryScanProgressChanged: {
            console.log("audio scan progress " + service.audioLibraryScanProgress)
            if(service.audioLibraryScanProgress == 0)
            {
                busyRefreshAudioLibrary.visible = false;
                btnRefreshAudioLibrary.visible = true;
                finishedRefreshAudioLibrary.visible = false;
            }
            else if(service.audioLibraryScanProgress == 100)
            {
                busyRefreshAudioLibrary.visible = false;
                btnRefreshAudioLibrary.visible = false;
                finishedRefreshAudioLibrary.visible = true;
                refreshAudioTimer.start()
            }
            else
            {
                busyRefreshAudioLibrary.visible = true;
                busyRefreshAudioLibrary.running = true;
                btnRefreshAudioLibrary.visible = false;
                finishedRefreshAudioLibrary.visible = false;
            }
        }
        onVideoLibraryScanProgressChanged: {
            console.log("video scan progress " + service.videoLibraryScanProgress)
            if(service.videoLibraryScanProgress == 0)
            {
                busyRefreshVideoLibrary.visible = false;
                btnRefreshVideoLibrary.visible = true;
                finishedRefreshVideoLibrary.visible = false;
            }
            else if(service.videoLibraryScanProgress == 100)
            {
                busyRefreshVideoLibrary.visible = false;
                btnRefreshVideoLibrary.visible = false;
                finishedRefreshVideoLibrary.visible = true;
                refreshVideoTimer.start()
            }
            else
            {
                busyRefreshVideoLibrary.visible = true;
                busyRefreshVideoLibrary.running = true;
                btnRefreshVideoLibrary.visible = false;
                finishedRefreshVideoLibrary.visible = false;
            }
        }
    }
    SystemService {
        client: appClient
        id: systemService
    }
}
