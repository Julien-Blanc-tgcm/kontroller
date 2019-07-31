import QtQuick 2.2
import QtQuick.Controls 2.2
import eu.tgcm 1.0
import "."
Rectangle {
    color:"#000"
    anchors.fill: parent
    id: main

    Remote {
        id:remote
    }

    Keys.onVolumeUpPressed: {
            remote.volumeUp();
    }
    Keys.onVolumeDownPressed: {
            remote.volumeDown();
    }

    StatusService {
        id:status
    }

/*    Flickable {
        anchors.fill: parent
        PullDownMenu {
            visible: status.servers.length > 1
            Repeater {
                model:status.servers
                delegate: MenuItem {
                    text:modelData.name
                    onClicked: {
                        status.switchToServer(modelData.name)
                    }
                }
            }
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Connected to %1").arg(status.server)
            visible: status.connectionStatus === 2
            height:Theme.itemSizeSmall
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Row {
            visible:status.connectionStatus === 0
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.horizontalPageMargin
            height:Theme.itemSizeSmall
            Label {
                height:Theme.itemSizeSmall
                text:qsTr("Unable to connect to %1").arg(status.server)
                color: Theme.highlightColor;
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            IconButton {
                icon.source: "image://theme/icon-m-refresh"
                onClicked: status.retryConnect();
            }
        }
        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            text:qsTr("Trying to connect to %1").arg(status.server)
            visible:status.connectionStatus === 1
            height:Theme.itemSizeSmall
            verticalAlignment: Text.AlignVCenter
            color: Theme.highlightColor
            horizontalAlignment: Text.AlignHCenter
        }
        Label {
            anchors.left: parent.left
            anchors.right: parent.right
            text: qsTr("Connection status : %1").arg(status.connectionStatus)
            visible: status.connectionStatus !== 0 && status.connectionStatus !== 1 && status.connectionStatus !== 2
            height: Theme.itemSizeSmall
            verticalAlignment: Text.AlignVCenter
            color:Theme.highlightColor
            horizontalAlignment: Text.AlignHCenter
        } */

        GridView {
            id:theGrid
            anchors.fill: parent
            anchors.topMargin:Theme.itemSizeSmall
            cellWidth:theGrid.width / 2
            cellHeight:cellWidth
            model : ListModel {
                ListElement{
                    page:"music"
                    icon:"image://theme/icon-m-music"
                    needConnect:true
                }
                ListElement{
                    page:"videos"
                    icon:"image://theme/icon-m-video"
                    needConnect:true
                }
                ListElement{
                    page:"current"
                    icon:"image://theme/icon-m-accessory-speaker"
                    needConnect:true
                }
                ListElement {
                    page:"remote"
                    icon:"image://theme/icon-m-game-controller"
                    needConnect:true
                }

                ListElement {
                    page:"playlist"
                    icon:"image://theme/icon-m-clipboard"
                    needConnect:true
                }

                ListElement{
                    page:"settings"
                    icon:"image://theme/icon-s-setting"
                    needConnect:false
                }

            }
            delegate : Item {
                Rectangle {
                    width: theGrid.width / 2 - 20
                    height: width
                    opacity:0.1
                    color:"#fff"
                    id:theRect
                    radius : 15
                    x:10
                    y:10
                }

                Button {
                    anchors.horizontalCenter: theRect.horizontalCenter
                    anchors.top:theRect.top
                    anchors.topMargin: 20
                    icon.source: {
                        if(!model.needConnect || status.connectionStatus === 2)
                            return model.icon;
                        else
                            return model.icon + "?" + theme.highlightDimmerColor;
                    }
                    onClicked: {
                        if(!model.needConnect || status.connectionStatus === 2)
                            pushRelevantPage(model.page)
                    }
                    id:btn
                }
                Label {
                    anchors.top:btn.bottom
                    anchors.topMargin:20
                    anchors.left: theRect.left
                    anchors.right: theRect.right
                    horizontalAlignment: Text.AlignHCenter
                    text: page
                    font.pixelSize: theme.fontSizeNormal
                    MouseArea
                    {
                        anchors.fill:parent
                        onClicked: {
                            if(!model.needConnect || status.connectionStatus === 2)
                                pushRelevantPage(page);
                        }
                    }
                    color:{
                        if(!model.needConnect || status.connectionStatus === 2)
                            return theme.primaryColor;
                        else
                            return theme.highlightDimmerColor
                    }
                }
            }
        }
//    }


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
                    "visible": true,
                    "anchors.fill":pageStack,
                    "browsingMode": filetype,
                    "browsingValue":file,
                    "label":label
                });
            musicList.mediaClicked.connect(createMusicPage);
            musicList.mediaInformationClicked.connect(createInformationPage);
//            musicList.remoteClicked.connect(pushRemotePage);
//            musicList.currentClicked.connect(pushCurrentPage);
//            musicList.backToMenuClicked.connect(toMenu);
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
                    "anchors.fill":pageStack,
                    "browsingMode":filetype,
                    "browsingValue":file,
                    "label":label
                });
            videoList.mediaClicked.connect(createVideoPage);
            videoList.mediaInformationClicked.connect(createInformationPage);
//            videoList.remoteClicked.connect(pushRemotePage);
//            videoList.currentClicked.connect(pushCurrentPage);
//            videoList.backToMenuClicked.connect(toMenu);
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
            internal.informationPageComponents[informationPageComponents[i].type] =
                    Qt.createComponent(informationPageComponents[i].component + '.qml');
        }
    }

    Component.onCompleted: createInfoComponents()
}
