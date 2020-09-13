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
                text: qsTr("settings")
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
                    label: qsTr("music")
                }
                ListElement {
                    page:"videos"
                    icon:"image://theme/icon-m-video"
                    needConnect:true
                    label: qsTr("videos")
                }
                ListElement {
                    page:"current"
                    icon:"image://theme/icon-m-accessory-speaker"
                    needConnect:true
                    label: qsTr("current")
                }
                ListElement {
                    page:"remote"
                    icon:"image://theme/icon-m-traffic"
                    needConnect:true
                    label: qsTr("remote")
                }

                ListElement {
                    page:"playlist"
                    icon:"image://theme/icon-m-menu"
                    needConnect:true
                    label: qsTr("playlist")
                }

                ListElement {
                    page: "server"
                    icon: "image://theme/icon-m-levels"
                    needConnect: true
                    label: qsTr("server management")
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

        // connecting
        Column {
            anchors.fill: parent
            anchors.topMargin: Theme.itemSizeExtraLarge
            spacing: Theme.itemSizeMedium
            visible: appClient.connectionStatus === 1
            // conneting
            Label {
                text: qsTr("Connecting, please wait");
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.Wrap
            }
            BusyIndicator {
                size: BusyIndicatorSize.Large
                running: appClient.connectionStatus === 1
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        // failed to connect
        Column {
            anchors.fill: parent
            anchors.topMargin: Theme.itemSizeExtraLarge
            spacing: Theme.itemSizeMedium
            visible: appClient.connectionStatus < 0 && appSettings.servers.length > 0
            // conneting
            Label {
                text: qsTr("Failed to connect");
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.Wrap
            }
            Button {
                text: qsTr("Retry")
                onClicked: appClient.refresh()
                anchors.horizontalCenter: parent.horizontalCenter
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

    QtObject {
        id:internal
        property var musicPageComponent:null
        property var videoPageComponent:null
        property var informationPageComponents

        function createMusicPage(file) {
            if(!!informationPageComponents[file.filetype])
            {
                createInformationPage(file);
                return;
            }
            if(!musicPageComponent)
                musicPageComponent = Qt.createComponent("MusicPage.qml");
            if(musicPageComponent.status !== Component.Ready)
            {
                console.log("Error at component creation");
                console.log(musicPageComponent.errorString());
                return;
            }
            var musicList = musicPageComponent.createObject(pageStack,
                {
                    visible: true,
                    browsingMode: file.filetype,
                    browsingValue:file.file,
                    label:file.label
                });
            musicList.mediaClicked.connect(createMusicPage);
            musicList.mediaInformationClicked.connect(createInformationPage);
            musicList.remoteClicked.connect(pushRemotePage);
            musicList.currentClicked.connect(pushCurrentPage);
            musicList.backToMenuClicked.connect(toMenu);
            pageStack.push(musicList);
        }

        function createInformationPage(file, creationData)
        {
            var newView;
            var component = internal.informationPageComponents[file.filetype];
            if(component && component.status === Component.Ready)
            {
                console.log("Create page for item ", file.id, " of type ", file.filetype);
                var data = {"itemId":file.id};
                if(creationData !== null)
                {
                    // extradata is needed for tv show informations (need both the tv show and the season parameter)
                    for(var key in creationData)
                        data[key] = creationData[key]; // copy data to creation parameters
                }

                newView = component.createObject(pageStack, data);
                if(newView)
                {
                    if(typeof(newView.mediaInformationClicked) !== "undefined")
                        newView.mediaInformationClicked.connect(createInformationPage);
                    if(typeof(newView.remoteClicked) !== "undefined")
                        newView.remoteClicked.connect(pushRemotePage);
                    if(typeof(newView.currentClicked) !== "undefined")
                        newView.currentClicked.connect(pushCurrentPage);
                    if(typeof(newView.backToMenuClicked) !== "undefined")
                        newView.backToMenuClicked.connect(toMenu);
                    //newView.label = pageStack.currentItem.label + "/" + label;
                    newView.label = file.label;
                }
                else
                    console.log("Error at view creation : " + component.errorString())
            }
            else if(component)
            {
                console.log(file.filetype + " " + component.errorString());
            }
            else
            {
                console.log("no component defined for " + file.filetype);
            }
            if(newView)
                pageStack.push(newView);
        }

        function createVideoPage(file) {
            if(!!informationPageComponents[file.filetype])
            {
                createInformationPage(file);
                return;
            }
            if(!videoPageComponent)
                videoPageComponent = Qt.createComponent("VideoPage.qml");
            if(videoPageComponent.status !== Component.Ready)
            {
                console.log("Error at video component creation");
                console.log(videoPageComponent.errorString());
                return;
            }
            var videoList = videoPageComponent.createObject(pageStack,
                {
                    "visible":true,
                    "browsingMode":file.filetype,
                    "browsingValue":file.file,
                    "label":file.label
                });
            videoList.mediaClicked.connect(createVideoPage);
            videoList.mediaInformationClicked.connect(createInformationPage);
            videoList.remoteClicked.connect(pushRemotePage);
            videoList.currentClicked.connect(pushCurrentPage);
            videoList.backToMenuClicked.connect(toMenu);
            pageStack.push(videoList);
        }
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
            pageStack.push(Qt.resolvedUrl("CurrentlyPlaying.qml"));
            pageStack.pushAttached(Qt.resolvedUrl("RemoteControl.qml"));
        }
        else if(page === "remote")
        {
            pageStack.push(Qt.resolvedUrl("RemoteControl.qml"))
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
        pageStack.pushAttached(Qt.resolvedUrl("RemoteControl.qml"));
        pageStack.navigateForward(PageStackAction.Animated)
    }

    function pushCurrentPage()
    {
        pageStack.pushAttached(Qt.resolvedUrl("CurrentlyPlaying.qml"));
        pageStack.navigateForward(PageStackAction.Animated)
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

}
