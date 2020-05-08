import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0

Page {
    id:main
    property string browsingMode: ""
    property string browsingValue: ""
    property string label: ""


    signal mediaClicked(string filetype, string file, string label)
    signal mediaPlayClicked(string dir)
    signal mediaInformationClicked(string filetype, string file, string label)
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    MusicService {
        id: musicService
        client: appClient
        browsingMode: main.browsingMode
        browsingValue: main.browsingValue
    }
    MusicControl {
        id: musicControl
        client: appClient
    }

    PlayableItemPage {
        id:theList
        service:musicService;
        control:musicControl
        anchors.fill: parent
        directPlay: ['album', 'directory', 'artist', 'media']
        playableItems :['directory', 'album', 'song', 'file']
        mediaType: "music"
        header:PageHeader {
            title:label
        }
    }


    Component.onCompleted: refresh()

    function refresh()
    {
        musicService.refresh()
    }

    function refreshCollection()
    {
        musicService.refreshCollection();
    }

}
