import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0

Page {
    id: __root
    property string browsingMode: ""
    property string browsingValue: ""
    property string label: ""


    signal mediaClicked(var file)
    signal mediaPlayClicked(string dir)
    signal mediaInformationClicked(var file)
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    MusicService {
        id: musicService
        client: appClient
        browsingMode: __root.browsingMode
        browsingValue: __root.browsingValue
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
        playableItems :['album', 'song', 'file']
        mediaType: "music"
        title: label

    }


    Component.onCompleted: refresh()

    function refresh()
    {
        musicService.refresh()
    }
}
