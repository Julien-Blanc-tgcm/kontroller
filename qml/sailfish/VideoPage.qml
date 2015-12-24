import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0

Page {
    id:main
    property alias browsingMode: videoService.browsingMode
    property alias browsingValue: videoService.browsingValue
    property alias label: videoService.label

    signal mediaClicked(string filetype, string file, string label)
    signal mediaPlayClicked(string dir)
    signal mediaInformationClicked(string filetype, string file, string label)

    VideoService {
        id: videoService
    }
    VideoControl {
        id: videoControl
    }

    PlayableItemPage {
        id:theList
        service: videoService
        control: videoControl
        anchors.fill: parent
        directPlay: ['directory', 'media', 'tvshow', 'season', 'movie']
        playableItems : ['movie', 'file', 'episode', 'musicvideo']
        header: PageHeader {
            title:label
        }
    }

    function refresh()
    {
        videoService.refresh()
    }

    Component.onCompleted : refresh()

    function refreshCollection()
    {
        videoService.refreshCollection();
    }

}
