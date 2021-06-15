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

    ImageService {
        id: imageService
        client: appClient
        browsingMode: __root.browsingMode
        browsingValue: __root.browsingValue
    }
    ImageControl {
        id: imageControl
        client: appClient
    }

    PlayableItemPage {
        id:theList
        service:imageService;
        control:imageControl
        anchors.fill: parent
        playableItems :['image']
        mediaType: "image"
        title: label
    }


    Component.onCompleted: refresh()

    function refresh()
    {
        imageService.refresh()
    }
}
