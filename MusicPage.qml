import QtQuick 2.0

import eu.tgcm 1.0

Item {
    id:main
    property alias browsingMode: musicService.browsingMode
    property alias browsingValue: musicService.browsingValue
    property alias label: musicService.label
    signal fileClicked(string file)
    signal mediaClicked(string filetype, string file, string label)
    signal songClicked(string song)
    signal playDirectoryAsked(string dir)

    MusicService {
        id: musicService
    }

    PlayableItemPage {
        id:theList
        service:musicService;
        anchors.fill: parent
    }

    Component.onCompleted: refresh()

    function refresh()
    {
        musicService.refresh()
    }

}
