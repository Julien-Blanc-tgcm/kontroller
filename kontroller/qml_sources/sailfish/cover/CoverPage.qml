import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Column {
        anchors.fill: parent
        anchors.margins: Theme.paddingLarge*2
        spacing: Theme.paddingLarge

        Label {
            id: label
            text: "Musikilo"
            font.pixelSize: Theme.fontSizeLarge
            font.bold: Font.Bold
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label {
            id: title
            width: parent.width
//            text: mediaPlayer.metaData.title !== undefined ? mediaPlayer.metaData.title : ""
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
            font.bold: Font.Bold
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: author
            width: parent.width
//            text: mediaPlayer.metaData.author!== undefined ?
//                      mediaPlayer.metaData.author :
//                      mediaPlayer.metaData.albumArtist !== undefined ?
//                          mediaPlayer.metaData.albumArtist :
//                          mediaPlayer.metaData.contributingArtist !== undefined ?
//                              mediaPlayer.metaData.contributingArtist : ""
            font.bold: Font.Bold
            horizontalAlignment: Text.AlignHCenter
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
        }
    }
}
