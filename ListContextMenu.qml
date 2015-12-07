import QtQuick 2.0

Item {
    states : [
        State {
            name: "album"
            PropertyChanges {
                target:albumContextMenu
                visible:true
            }
        },
        State {
            name: "video"
            PropertyChanges {
                target:videoContextMenu
                visible:true
            }
        },
        State {
            name : "image"
        },
        State {
            name : "artist"
            PropertyChanges {
                target: artistContextMenu
                visible : true
            }
        },
        State {
            name: "song"
            PropertyChanges {
                target: songContextMenu
                visible:true
            }
        },
        State {
            name: "audiodirectory"
            PropertyChanges {
                target: audioDirectoryContextMenu
                visible:true
            }
        }

    ]
    id:main
    property string color;
    property alias border : albumContextMenu.border;
    state: ""

    Rectangle {
        height : txtAddToPlaylist.height * 5
        width: Math.max(txtAddToPlaylist.contentWidth, playNow.contentWidth) + 20
        id:albumContextMenu
        visible: false
        color:parent.color
        Text {
            id:txtAddToPlaylist
            color:"#00b"
            text: qsTr("Add to playlist")
            anchors.left: parent.left;
            anchors.leftMargin: 10 * scalingFactor
            anchors.top: parent.top
            anchors.topMargin: height / 2
            font.pixelSize: 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: addToPlaylistPressed()
            }
        }
        Text {
            id:playNow
            color:"#00b"
            text: qsTr("Play immediately")
            anchors.left: parent.left
            anchors.leftMargin: txtAddToPlaylist.anchors.leftMargin
            anchors.top : txtAddToPlaylist.bottom
            anchors.topMargin: height / 2
            font.pixelSize: 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: playPressed()
            }
        }
        Text {
            id:txtInformation
            color:"#00b"
            text: qsTr("View information")
            anchors.left: parent.left
            anchors.leftMargin: txtAddToPlaylist.anchors.leftMargin
            anchors.top : playNow.bottom
            anchors.topMargin: height / 2
            font.pixelSize : 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: informationPressed()
            }
        }
    }

    Rectangle {
        height : txtAddToPlaylist.height * 5
        width: Math.max(txtVideoAddToPlaylist.contentWidth, playNow.contentWidth) + 20
        color:parent.color
        border.color: albumContextMenu.border.color
        border.width: albumContextMenu.border.width
        visible:false
        id:videoContextMenu
        Text {
            id:txtVideoAddToPlaylist
            color:"#00b"
            text: qsTr("Add to playlist")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top:parent.top
            anchors.topMargin: height/2
            font.pixelSize : 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: addToPlaylistPressed()
            }
        }
        Text {
            id:videoPlayNow
            color:"#00b"
            text: qsTr("Play immediately")
            anchors.left: parent.left
            anchors.leftMargin: txtVideoAddToPlaylist.anchors.leftMargin
            anchors.top : txtVideoAddToPlaylist.bottom
            anchors.topMargin: height / 2
            font.pixelSize : 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: playPressed()
            }
        }
        Text {
            id:txtVideoInformation
            color:"#00b"
            text: qsTr("View information")
            anchors.left: parent.left
            anchors.leftMargin: txtVideoAddToPlaylist.anchors.leftMargin
            anchors.top : videoPlayNow.bottom
            anchors.topMargin: height / 2
            font.pixelSize : 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: informationPressed()
            }
        }
    }

    Rectangle {
        height : txtArtistInformation.height * 2
        width: txtArtistInformation.contentWidth + 20
        color:parent.color
        border.color: albumContextMenu.border.color
        border.width: albumContextMenu.border.width
        visible:false
        id:artistContextMenu
        Text {
            id:txtArtistInformation
            color:"#00b"
            text: qsTr("View artist information")
            anchors.left: parent.left
            anchors.leftMargin: txtAddToPlaylist.anchors.leftMargin
            anchors.top : parent.top
            anchors.topMargin: height / 2
            font.pixelSize : 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: informationPressed()
            }
        }
    }

    Rectangle {
        height : txtAddToPlaylist.height * 2
        width: Math.max(songAddToPlaylist.contentWidth, songPlayNow.contentWidth) + 20
        color:parent.color
        border.color: albumContextMenu.border.color
        border.width: albumContextMenu.border.width
        visible:false
        id:songContextMenu
        Text {
            id:songAddToPlaylist
            color:"#00b"
            text: qsTr("Add to playlist")
            anchors.left: parent.left;
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: height / 2
            font.pixelSize : 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: addToPlaylistPressed()
            }
        }
        Text {
            id:songPlayNow
            color:"#00b"
            text: qsTr("Play immediately")
            anchors.left: parent.left
            anchors.leftMargin: songAddToPlaylist.anchors.leftMargin
            anchors.top : songAddToPlaylist.bottom
            anchors.topMargin: height / 2
            font.pixelSize : 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: playPressed()
            }
        }
    }

    Rectangle {
        id:audioDirectoryContextMenu
        height : txtAddToPlaylist.height * 3.5
        width: Math.max(audioDirectoryAddToPlaylist.contentWidth, audioDirectoryPlayNow.contentWidth) + 20
        color:parent.color
        border.color: albumContextMenu.border.color
        border.width: albumContextMenu.border.width
        visible:false
        Text {
            id:audioDirectoryAddToPlaylist
            color:"#00b"
            text: qsTr("Add to playlist")
            anchors.left: parent.left;
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: height / 2
            font.pixelSize : 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: addToPlaylistPressed()
            }
        }
        Text {
            id:audioDirectoryPlayNow
            color:"#00b"
            text: qsTr("Play immediately")
            anchors.left: parent.left
            anchors.leftMargin: audioDirectoryAddToPlaylist.anchors.leftMargin
            anchors.top : audioDirectoryAddToPlaylist.bottom
            anchors.topMargin: height / 2
            font.pixelSize : 12 * scalingFactor
            MouseArea {
                anchors.fill: parent
                onClicked: playPressed()
            }
        }
    }

    signal playPressed()
    signal addToPlaylistPressed()
    signal informationPressed()
}

