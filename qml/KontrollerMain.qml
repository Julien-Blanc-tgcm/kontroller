import QtQuick 2.6
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."
import org.nemomobile.policy 1.0
import Sailfish.Media 1.0

Page {
    id: main

    Remote {
        id:remote
        client: appClient
    }

    MediaKey {
        enabled: Qt.application.active
        key: Qt.Key_VolumeUp
        onPressed: remote.volumeUp()
        onRepeat: remote.volumeUp()
    }
    MediaKey {
        enabled: Qt.application.active
        key: Qt.Key_VolumeDown
        onPressed: remote.volumeDown()
        onRepeat: remote.volumeDown()
    }
    Permissions {
        enabled: Qt.application.active
        autoRelease: true
        applicationClass: "camera"

        Resource {
            id: volumeKeysResource
            type: Resource.ScaleButton
            optional: true
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("SettingsPage.qml"));
                }
            }

            MenuItem {
                visible: appSettings.servers.length > 1
                text: qsTr("Switch server")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("ServerSelectionPage.qml"));
                }
            }
        }

        PageHeader {
            id: titleBar
            title: (appSettings.servers.length >= 1 && appClient.server) ?
                       appClient.server.name :
                       qsTr("Welcome")
        }

        SilicaListView
        {
            anchors.top: titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            visible: appClient.connectionStatus === 2
            model: ListModel {
                ListElement {
                    page:"music"
                    icon:"image://theme/icon-m-media-songs"
                    needConnect:true
                    label: qsTr("Music")
                }
                ListElement {
                    page:"videos"
                    icon:"image://assets/icons/icon-m-video-k"
                    needConnect:true
                    label: qsTr("Videos")
                }
                ListElement {
                    page:"images"
                    icon:"image://theme/icon-m-image"
                    needConnect: true
                    label: qsTr("Images")
                }
                ListElement {
                    page:"current"
                    icon:"image://theme/icon-m-accessory-speaker"
                    needConnect:true
                    label: qsTr("Current")
                }
                ListElement {
                    page:"remote"
                    icon:"image://assets/icons/icon-m-remote"
                    needConnect:true
                    label: qsTr("Remote")
                }

                ListElement {
                    page:"playlist"
                    icon:"image://theme/icon-m-menu"
                    needConnect:true
                    label: qsTr("Playlist")
                }

                ListElement {
                    page: "server"
                    icon: "image://theme/icon-m-levels"
                    needConnect: true
                    label: qsTr("Server management")
                }
            }
            delegate: ListItem {
                contentHeight: Theme.itemSizeLarge
                Row {
                    leftPadding: Theme.itemSizeLarge
                    spacing: Theme.horizontalPageMargin
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    IconButton {
                        id: img
                        icon.source: {
                            if(!model.needConnect || appClient.connectionStatus === 2)
                                return model.icon;
                            else
                                return model.icon + "?" + Theme.highlightDimmerColor;
                        }
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: model.label
                        color: (appClient.connectionStatus === 2)?
                                   Theme.primaryColor :
                                   Theme.highlightDimmerColor;
                    }
                }
                onClicked: {
                    if(!model.needConnect || appClient.connectionStatus === 2)
                        pushRelevantPage(model.page);
                }
            }
        }

        // connecting / connection failed
        Column {
            anchors.fill: parent
            anchors.topMargin: Theme.itemSizeExtraLarge
            spacing: Theme.itemSizeMedium
            visible: (appClient.connectionStatus === 1 || appClient.connectionStatus === -1) &&
                     appSettings.servers.length > 0
            // connecting
            Label {
                visible: appClient.connectionStatus === 1
                text: qsTr("Connecting…");
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.Wrap
            }
            BusyIndicator {
                visible: running
                size: BusyIndicatorSize.Large
                running: appClient.connectionStatus === 1
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // failed to connect
            Label {
                visible: appClient.connectionStatus === -1
                text: qsTr("Could not connect");
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.Wrap
            }
            Button {
                visible: appClient.connectionStatus === -1 || appClient.connectionStatus === 1
                text: qsTr("Retry")
                onClicked: appClient.refresh()
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // always visible
            Button {
                text: qsTr("Wake up server")
                onClicked: appClient.wakeUp();
                anchors.horizontalCenter: parent.horizontalCenter
                visible: appClient && appClient.wakeUpPlugin
            }
        }

        // connecting / connection failed
        Column {
            anchors.fill: parent
            anchors.topMargin: Theme.itemSizeExtraLarge
            spacing: Theme.itemSizeMedium
            visible: (appClient.connectionStatus === -2) && appSettings.servers.length > 0
            // connecting
            Label {
                text: qsTr("Wifi is disabled, enable it to connect");
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                width: parent.width
            }
        }

        // no server configured
        Column {
            anchors.fill: parent
            anchors.topMargin: Theme.itemSizeExtraLarge
            spacing: Theme.itemSizeMedium
            visible: appSettings.servers.length === 0
            // conneting
            Label {
                text: qsTr("No server configured");
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.Wrap
            }
            Button {
                text: qsTr("Add server")
                onClicked: {
                    var newserveruuid = appSettings.newServer().uuid; // will give a uuid
                    pushServerSettingsPage(newserveruuid); // go to settings page
                }

                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

    }

    PageFactory {
        id:internal

    }


    function pushRelevantPage(page)
    {
        if(page === "music")
        {
            internal.createMusicPage({"file": "", "filetype": "", "label": qsTr("Sources")});
        }
        else if(page === "videos")
        {
            internal.createVideoPage({"file": "", "filetype": "", "label": qsTr("Sources")});
        }
        else if(page === "images")
        {
            internal.createImagePage({"file":"", "filetype":"", "label":qsTr("Sources")});
        }
        else if(page === "current")
        {
            internal.createCurrentlyPlayingPage();
        }
        else if(page === "remote")
        {
            pushRemotePage()
        }
        else if(page === "playlist")
        {
            pageStack.push(Qt.resolvedUrl("PlaylistPage.qml"));
        }
        else if(page === "server")
        {
            pageStack.push(Qt.resolvedUrl("ServerPage.qml"))
        }
    }

    function pushRemotePage()
    {
        pageStack.push(Qt.resolvedUrl("RemoteControl.qml"));
    }

    function pushCurrentPage()
    {
        pageStack.push(Qt.resolvedUrl("CurrentlyPlaying.qml"));
    }

    function toMenu()
    {
        pageStack.pop(main)
    }

    function createInfoComponents() {
        internal.informationPageComponents = {};
        var informationPageComponents = [{"type":"album", "component":"AlbumInformationPage"},
                {"type":"artist", "component":"ArtistInformationPage"},
                {"type":"episode", "component":"EpisodeInformationPage"},
                {"type":"movie", "component":"MovieInformationPage"},
                {"type":"season", "component":"SeasonInformationPage"},
                {"type":"tvshow", "component":"TvShowInformationPage"}];
        for(var i = 0; i < informationPageComponents.length; ++i)
        {
            var comp = Qt.createComponent(informationPageComponents[i].component + '.qml');
            internal.informationPageComponents[informationPageComponents[i].type] = comp;
        }
    }

    Component.onCompleted:{
        if(appSettings.lastServer.length > 0)
            appClient.switchToServer(appSettings.lastServer)
        createInfoComponents()
        serverSettingsComponent_ = Qt.createComponent(Qt.resolvedUrl("ServerSettingsPage.qml"))
    }

    Connections {
        target: appSettings
        onServersChanged: {
            if(appSettings.servers.length === 1)
            {
                var server = appSettings.servers[0];
                appClient.switchToServer(server.uuid);
                appSettings.lastServer = server.uuid
            }
        }
    }

    onStatusChanged: {
        if(status == PageStatus.Activating &&
                appClient.connectionStatus !== 1 &&
                appClient.connectionStatus !== 2)
        {
            appClient.retryConnect();
        }
    }

    property var serverSettingsComponent_

    function pushServerSettingsPage(serveruuid)
    {
        console.log("push server settings for server " + serveruuid);
        if(serverSettingsComponent_.status === Component.Error)
        console.log(serverSettingsComponent_.errorString())
        pageStack.push(serverSettingsComponent_.createObject(pageStack,
                                                             {"serverUuid": serveruuid,
                                                              "newServer": true}))
    }

    function pushCurrentlyPlaying()
    {
        internal.createCurrentlyPlayingPage();
    }

}
