import QtQuick 2.0

Rectangle {
    states : [
        State {
            name: "audio"
        },
        State {
            name: "video"
        },
        State {
            name : "image"
        }
    ]
    height : txtAddToPlaylist.height * 3.5
    width: Math.max(txtAddToPlaylist.contentWidth, playNow.contentWidth) + 20
    state: "audio"


    Text {
        id:txtAddToPlaylist
        color:"#00b"
        text: qsTr("Add to playlist")
        anchors.left: parent.left;
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: height / 2
        visible:state == "audio"
    }
    Text {
        id:playNow
        color:"#00b"
        text: qsTr("Play immediately")
        anchors.left: parent.left
        anchors.leftMargin: txtAddToPlaylist.anchors.leftMargin
        anchors.top : txtAddToPlaylist.bottom
        anchors.topMargin: height / 2
    }

    signal playPressed()
    signal addToPlaylistPressed()
}

