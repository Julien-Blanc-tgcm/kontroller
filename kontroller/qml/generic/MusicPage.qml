import QtQuick 2.0

import eu.tgcm 1.0

Rectangle {
    id:main
    property alias browsingMode: musicService.browsingMode
    property alias browsingValue: musicService.browsingValue
    property alias label: musicService.label
    //signal fileClicked(string file)
    signal mediaClicked(string filetype, string file, string label)
    //signal songClicked(string song)
    signal mediaPlayClicked(string dir)
    signal mediaInformationClicked(string filetype, string file, string label)
    anchors.fill: parent
    color:theme.backgroundColor

    MusicService {
        id: musicService
        client: appClient
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
    }

    Component.onCompleted: refresh()

    function refresh()
    {
        musicService.refresh()
    }

}
