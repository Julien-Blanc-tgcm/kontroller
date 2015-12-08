import QtQuick 2.0

import eu.tgcm 1.0

Item {
    id:main
    property alias browsingMode: videoService.browsingMode
    property alias browsingValue: videoService.browsingValue
    property alias label: videoService.label
    signal fileClicked(string file)
    signal mediaClicked(string filetype, string file, string label)

    VideoService {
        id: videoService
    }

    PlayableItemPage {
        id:theList
        service: videoService
        anchors.fill: parent
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
