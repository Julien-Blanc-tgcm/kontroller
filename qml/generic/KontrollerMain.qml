import QtQuick 2.2
import eu.tgcm 1.0
import "."
Rectangle {
    color:"#000"
    anchors.fill: parent
    id: main

    RemoteControl {
        id:remote
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    }

    CurrentlyPlaying {
        id : currentplaying
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible:false
    }

    SettingsPage {
        id:settings
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible:false
    }

    property var musicList;
    property var videosList;
    QtObject {
        id: creator
        property var listComponent;
        function hideAllPages() {
            if(musicList)
            {
                musicList.visible = false;
                musicList.focus = false;
            }
            if(videosList)
            {
                videosList.visible = false;
                videosList.focus = false;
            }
            if(remote)
            {
                remote.visible = false;
                remote.focus = false;
            }
            if(settings)
            {
                settings.visible = false
                settings.focus = false
            }
            if(currentplaying)
            {
                currentplaying.visible = false
                currentplaying.focus = false
            }
        }
    }


    Hideable {
        id:flickable
        anchorRaised:main.top
        anchorFlicked:main.bottom
        anchors.left: main.left
        anchors.right: main.right
        anchors.bottom: main.bottom
    }

    function activateSettingsPage() {
        creator.hideAllPages()
        settings.visible = true;
    }

    function activateMusicPage() {
        creator.hideAllPages()
        if(main.musicList)
        {
            main.musicList.visible = true;
            main.musicList.focus = true;
            return;
        }

        if(!creator.listComponent)
            creator.listComponent = Qt.createComponent("GenericList.qml");
        if(creator.listComponent.status === Component.Ready)
        {
            musicList = creator.listComponent.createObject(main,
                                                               {
                                                               "visible": true,
                                                               "anchors.fill":main,
                                                               "pageComponent": "MusicPage",
                                                               "informationPageComponents": [
                                                                  {'type':'artist', 'component':'ArtistInformationPage'},
                                                                  {'type':'album', 'component':'AlbumInformationPage'}
                                                              ],
                                                              "defaultsToInformation": ['artist', 'album']
                                                               });
            musicList.focus = true;
        }
        else
            console.log(creator.listComponent.errorString());
    }

    function activateVideosPage() {
        creator.hideAllPages()
        if(videosList)
        {
            videosList.visible = true;
            videosList.focus = true;
            return
        }
        if(!creator.listComponent)
            creator.listComponent = Qt.createComponent("GenericList.qml");
        if(creator.listComponent.status === Component.Ready)
        {
            videosList = creator.listComponent.createObject(main,
                                                               {
                                                               "visible": true,
                                                               "anchors.fill":main,
                                                               "pageComponent": "VideoPage",
                                                                "informationPageComponents": [
                                                                    {'type':'movie', 'component':'MovieInformationPage'},
                                                                    {'type':'tvshow', 'component' : 'TvShowInformationPage'},
                                                                    {'type':'season', 'component' : 'SeasonInformationPage'},
                                                                    {'type':'episode', 'component': 'EpisodeInformationPage'}
                                                                ],
                                                                "defaultsToInformation": ['movie', 'tvshow']
                                                            });
            videosList.focus = true;
        }
        else
            console.log(creator.listComponent.errorString());
    }

    function activateRemotePage() {
        creator.hideAllPages()
        remote.visible = true;
    }

    function activateCurrentlyPlayingPage() {
        creator.hideAllPages()
        currentplaying.visible = true;
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
}
