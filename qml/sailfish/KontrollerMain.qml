import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."

Page {
    anchors.fill: parent
    id: main

    SilicaGridView {
        id:theGrid
        anchors.fill: parent
        cellWidth:theGrid.width / 2
        cellHeight:cellWidth
        model : ListModel {
            ListElement{
                page:"music"
                icon:"image://theme/icon-m-music"
            }
            ListElement{
                page:"videos"
                icon:"image://theme/icon-m-video"
            }
            ListElement{
                page:"current"
                icon:"image://theme/icon-m-accessory-speaker"
            }
            ListElement {
                page:"remote"
                icon:"image://theme/icon-m-game-controller"
            }

            ListElement {
                page:"playlist"
                icon:"image://theme/icon-m-clipboard"
            }

            ListElement{
                page:"settings"
                icon:"image://theme/icon-s-setting"
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

            IconButton {
                anchors.horizontalCenter: theRect.horizontalCenter
                anchors.top:theRect.top
                anchors.topMargin: 20
                icon.source: model.icon
                onClicked: pushRelevantPage(model.page)
                id:btn
            }
            Label {
                anchors.top:btn.bottom
                anchors.topMargin:20
                anchors.left: theRect.left
                anchors.right: theRect.right
                horizontalAlignment: Text.AlignHCenter
                text: page
                MouseArea
                {
                    anchors.fill:parent
                    onClicked: pushRelevantPage(page);
                }
            }
        }
    }

    QtObject {
        id:internal
        property var musicPageComponent:null
        property var videoPageComponent:null

        function createMusicPage(fileType, file, label) {
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
                    "browsingMode": fileType,
                    "browsingValue":file,
                    "label":label
                });
            musicList.mediaClicked.connect(createMusicPage);
            musicList.mediaInformationClicked.connect(createInformationPage);
            pageStack.push(musicList);
        }

        function createInformationPage(filetype, file, label)
        {
            console.log("media info");
        }

        function createVideoPage(filetype, file, label) {
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

/*    RemoteControl {
        id:remote
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
    } */

/*    CurrentlyPlaying {
        id : currentplaying
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible:false
    } */

    /*SettingsPage {
        id:settings
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        visible:true
    }

    property var musicList;
    property var videosList; */


  /*  StatusBar {
        id:status
        anchors.bottom: parent.bottom
        height: computeStatusBarHeight()
        anchors.left: parent.left
        anchors.right: parent.right
        onPlaylistClicked: showPlaylist(type)
        onStateChanged: focusAccordingToState()
        z:2
    }*/
/*    Hideable {
        id:flickable
        anchorRaised:main.top
        anchorFlicked:main.bottom
        anchors.left: main.left
        anchors.right: main.right
        anchors.bottom: main.bottom
    } */

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
