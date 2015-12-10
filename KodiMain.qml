import QtQuick 2.2
import eu.tgcm 1.0

Rectangle {
    color:"#000"
    anchors.fill: parent
    id: main
    states: [
        State {
            name: "remote"
        },
        State {
            name: "settings"
        },
        State {
            name: "music"
            PropertyChanges {
                target: musicList
                focus:true
            }
        },
        State {
            name: "videos"
            PropertyChanges {
                target:videosList
                focus:true
            }
        },
        State {
            name: "other"
        }
    ]

    RemoteControl {
        visible: main.state === "remote" || main.state == ""
        state: "remote"
        anchors.top: parent.top
        anchors.bottom: status.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    SettingsPage {
        visible: main.state === "settings"
        anchors.top: parent.top
        anchors.bottom: status.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    GenericList {
        visible: main.state === "music"
        id:musicList
        anchors.top: parent.top
        anchors.bottom: status.top
        anchors.left: parent.left
        anchors.right: parent.right
        pageComponent: "MusicPage"
        informationPageComponents: [
            {'type':'artist', 'component':'ArtistInformationPage'},
            {'type':'album', 'component':'AlbumInformationPage'}
        ]
        defaultsToInformation: ['artist', 'album'];
    }

    GenericList {
        visible: main.state === "videos"
        id:videosList
        anchors.top:parent.top
        anchors.bottom: status.top
        anchors.left: parent.left
        anchors.right: parent.right
        pageComponent: "VideoPage"
        informationPageComponents: [
            {'type':'movie', 'component':'MovieInformationPage'}
        ]
        defaultsToInformation: ['movie']
    }

    StatusBar {
        id:status
        anchors.bottom: parent.bottom
        height: computeStatusBarHeight()
        anchors.left: parent.left
        anchors.right: parent.right
        onPlaylistClicked: showPlaylist(type)
        onStateChanged: focusAccordingToState()
        z:2
    }
    Hideable {
        id:flickable
        anchorRaised:main.top
        anchorFlicked:status.top
        anchors.left: main.left
        anchors.right: main.right
        anchors.bottom: status.top
        onStateChanged: {
            if(main.state === "music")
                musicList.focus = true;
            else if(main.state === "videos")
                videosList.focus = true;
        }
    }

    function activateSettingsPage() {
        main.state = "settings"
    }

    function activateMusicPage() {
        main.state = "music"
    }

    function activateVideosPage() {
        main.state = "videos"
    }

    function activateRemotePage() {
        main.state = "remote"
    }

    function createMusicListComponent() {
        if(active && musicTab.children.length === 0)
        {
            var theMusicTab = musicList.createObject(musicTab,
                                                              {
                                                                  "anchors.fill":musicTab,
                                                              });
        }
    }

    function computeStatusBarHeight()
    {
        if(main.width < main.height)
        {
            return main.width / 6;
        }
        else
        {
            return 40 * touchScalingFactor;
           // return main.height / 6;
        }
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
/*            sprite = component.createObject(appWindow, {"x": 100, "y": 100}); */
            else
            {
                showPlaylist_(type);
            }
        } else if (playlistPageComponent.status === Component.Error) {
            // Error Handling
            console.log("Error loading component:", playlistPageComponent.errorString());
        }
        else
            console.log("bordel");
    }
}
