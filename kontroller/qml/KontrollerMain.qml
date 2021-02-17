import QtQuick 2.6
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."

Page {
    id: main

    Remote {
        id:remote
        client: appClient
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
            visible: (appClient.connectionStatus === 1 || appClient.connectionStatus < 0) &&
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
                visible: appClient.connectionStatus < 0
                text: qsTr("Could not connect");
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.Wrap
            }
            Button {
                visible: appClient.connectionStatus < 0
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

    property var playlist;
    property var playlistPageComponent;

    function showPlaylist(type) {
        if(!playlist)
        {
            if(!playlistPageComponent)
            {
                playlistPageComponent = Qt.createComponent("PlaylistPage.qml");
                if (playlistPageComponent.status === Component.Ready)
                    finishCreation(type);
                else if(playlistPageComponent.status !== Component.Error)
                {
                    playlistPageComponent.statusChanged.connect(function() { finishCreation(type);});
                }
                else
                    console.log(playlistPageComponent.errorString())
            }
        }
        else
            showPlaylist_(type)
    }
    function showPlaylist_(type)
    {
        playlist.playlistType = type;
        if(flickable.state === "raised")
        {
            flickable.state = "lowered";
        }
        else
            flickable.state = "raised";
    }

    function finishCreation(type) {
        if (playlistPageComponent.status === Component.Ready) {
            playlist = playlistPageComponent.createObject(main,
                                                          {
                "anchors.fill": flickable,
                "playlistType":type});
            if (playlist === null) {
                // Error Handling
                console.log("Error creating object");
            }
            else
            {
                playlist.z = 1;
                showPlaylist_(type);
            }
        } else if (playlistPageComponent.status === Component.Error) {
            // Error Handling
            console.log("Error loading component:", playlistPageComponent.errorString());
        }
        else
            console.log("bordel");
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
