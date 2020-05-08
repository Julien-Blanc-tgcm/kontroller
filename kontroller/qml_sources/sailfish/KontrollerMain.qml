import QtQuick 2.0
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
            visible: appSettings.servers.length > 1
            Repeater {
                model:appSettings.servers
                delegate: MenuItem {
                    text:modelData.name
                    onClicked: {
                        appClient.switchToServer(modelData.uuid)
                        appSettings.lastServer = modelData.uuid
                    }
                }
            }
        }

        PushUpMenu {
            visible: true
            MenuItem {
                text: qsTr("About")
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("About.qml"))
                }
            }
        }

        Item {
            id:titleBar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:parent.top
            height: childrenRect.height
            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Connected to %1").arg(appClient.server?appClient.server.name:"")
                visible: appClient.connectionStatus === 2
                height:Theme.itemSizeSmall
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Row {
                visible:appClient.connectionStatus === 0
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                height:Theme.itemSizeSmall
                Label {
                    height:Theme.itemSizeSmall
                    text:qsTr("Unable to connect to %1").arg(appClient.server?appClient.server.name:"")
                    color: Theme.highlightColor;
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                IconButton {
                    icon.source: "image://theme/icon-m-refresh"
                    onClicked: appClient.retryConnect();
                }
            }
            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                text:qsTr("Trying to connect to %1").arg(appClient.server?appClient.server.name:"")
                visible: appClient.connectionStatus === 1
                height:Theme.itemSizeSmall
                verticalAlignment: Text.AlignVCenter
                color: Theme.highlightColor
                horizontalAlignment: Text.AlignHCenter
            }
            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("Connection status : %1").arg(appClient.connectionStatus)
                visible: appSettings.servers.length !== 0 &&
                    appClient.connectionStatus !== 0 && appClient.connectionStatus !== 1 &&
                         appClient.connectionStatus !== 2
                height: Theme.itemSizeSmall
                verticalAlignment: Text.AlignVCenter
                color:Theme.highlightColor
                horizontalAlignment: Text.AlignHCenter
            }
            Label {
                anchors.left: parent.left
                anchors.right: parent.right
                text: qsTr("No server configured")
                visible: appSettings.servers.length === 0
                height: Theme.itemSizeSmall
                verticalAlignment: Text.AlignVCenter
                color:Theme.highlightColor
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top:titleBar.bottom
            anchors.topMargin:Theme.paddingMedium
            id: theGrid
            property bool landscape: height < width
            property int cellWidth: landscape ? Math.min(theGrid.width / 3, theGrid.height / 2) :  Math.min(theGrid.width / 2, theGrid.height / 3)
            property int numberOfRows : landscape ? 2 : 3;
            property int numberOfCols : landscape ? 3 : 2;
            Repeater {
                model : ListModel {
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
                        page:"settings"
                        icon:"image://theme/icon-m-developer-mode"
                        needConnect:false
                        label: qsTr("settings")
                    }
                }

                delegate : Item {
                    property int positionX : (theGrid.landscape ? (model.index % 3) : (model.index % 2))
                    property int positionY : Math.floor(theGrid.landscape ? (model.index / 3) : (model.index / 2))
                    x: (main.width - (theGrid.cellWidth * theGrid.numberOfCols)) /2 + (positionX * theGrid.cellWidth)
                    y: positionY * theGrid.cellWidth
                    width: theGrid.cellWidth
                    height: theGrid.cellWidth
                    Rectangle {
                        anchors.fill: parent
                        opacity:0.1
                        color:"#fff"
                        id:theRect
                        radius : 15
                        anchors.margins: Theme.paddingMedium
                        MouseArea
                        {
                            anchors.fill:parent
                            onClicked: {
                                if(!model.needConnect || appClient.connectionStatus === 2)
                                    pushRelevantPage(model.page);
                            }
                        }
                    }
                    Column
                    {
                        anchors.verticalCenter: theRect.verticalCenter
                        spacing: Theme.paddingLarge
                        anchors.horizontalCenter: parent.horizontalCenter
                        IconButton {
                            anchors.horizontalCenter: parent.horizontalCenter
                            icon.source: {
                                if(!model.needConnect || appClient.connectionStatus === 2)
                                    return model.icon;
                                else
                                    return model.icon + "?" + Theme.highlightDimmerColor;
                            }
                            onClicked: {
                                if(!model.needConnect || appClient.connectionStatus === 2)
                                    pushRelevantPage(model.page)
                            }
                            id:btn
                            width:icon.width
                            height:icon.height
                        }
                        Label {
                            id:lbl
                            anchors.left:parent.left
                            anchors.right: parent.right
                            horizontalAlignment: Text.AlignHCenter
                            text: label
                            color:{
                                if(!model.needConnect || appClient.connectionStatus === 2)
                                    return Theme.primaryColor;
                                else
                                    return Theme.highlightDimmerColor
                            }
                        }
                    }
                }
            }
        }
    }

    QtObject {
        id:internal
        property var musicPageComponent:null
        property var videoPageComponent:null
        property var informationPageComponents

        function createMusicPage(filetype, file, label) {
            if(!!informationPageComponents[filetype])
            {
                createInformationPage(filetype, file, label);
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
                    browsingMode: filetype,
                    browsingValue:file,
                    label:label
                });
            musicList.mediaClicked.connect(createMusicPage);
            musicList.mediaInformationClicked.connect(createInformationPage);
            musicList.remoteClicked.connect(pushRemotePage);
            musicList.currentClicked.connect(pushCurrentPage);
            musicList.backToMenuClicked.connect(toMenu);
            pageStack.push(musicList);
        }

        function createInformationPage(filetype, file, label)
        {
            var newView;
            var component = internal.informationPageComponents[filetype];
            if(component && component.status === Component.Ready)
            {
                newView = component.createObject(pageStack, {"itemId":file});
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
                    newView.label = label;
                }
                else
                    console.log("Error at view creation : " + component.errorString())
            }
            else if(component)
            {
                console.log(filetype + " " + component.errorString());
            }
            else
            {
                console.log("no component defined for " + filetype);
            }
            if(newView)
                pageStack.push(newView);
        }

        function createVideoPage(filetype, file, label) {
            if(!!informationPageComponents[filetype])
            {
                createInformationPage(filetype, file, label);
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
                    "browsingMode":filetype,
                    "browsingValue":file,
                    "label":label
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
            internal.createMusicPage("", "", qsTr("Sources"));
        }
        else if(page === "videos")
        {
            internal.createVideoPage("","",qsTr("Sources"));
        }
        else if(page === "current")
        {
            pageStack.push(Qt.resolvedUrl("CurrentlyPlaying.qml"));
            pageStack.pushAttached(Qt.resolvedUrl("RemoteControl.qml"));
        }

        else if(page === "settings")
        {
            pageStack.push(Qt.resolvedUrl("SettingsPage.qml"));
        }
        else if(page === "remote")
        {
            pageStack.push(Qt.resolvedUrl("RemoteControl.qml"))
        }
        else if(page === "playlist")
        {
            pageStack.push(Qt.resolvedUrl("PlaylistPage.qml"));
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
    }
}
