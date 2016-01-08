import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0

Page {
    id:main
    property alias browsingMode: musicService.browsingMode
    property alias browsingValue: musicService.browsingValue
    property alias label: musicService.label
    signal mediaClicked(string filetype, string file, string label)
    signal mediaPlayClicked(string dir)
    signal mediaInformationClicked(string filetype, string file, string label)
    signal remoteClicked()
    signal currentClicked()
    signal backToMenuClicked()

    MusicService {
        id: musicService
    }
    MusicControl {
        id: musicControl
    }

/*    SilicaFlickable {
        anchors.fill: parent
        PageHeader {
            title:qsTr("Music")
            id:header
        } */

    PlayableItemPage {
        id:theList
        service:musicService;
        control:musicControl
        anchors.fill: parent
        directPlay: ['album', 'directory', 'artist', 'media']
        playableItems :['directory', 'album', 'song', 'file']
        header:PageHeader {
            title:label
        }
    }
//    }


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
