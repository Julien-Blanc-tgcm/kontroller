import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.6
import org.nemomobile.mpris 1.0
import "../components"

Page {
    id: mainPage

    allowedOrientations: Orientation.All

    SlideshowView {
        id: swipeView
        itemWidth: width
        itemHeight: height
        clip: true

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height - mainFlickable.height - mainFlickable.contentY

        model: VisualItemModel {
//            FilesList { }
//            SongsList { }
            SongDetails { }
            Settings { }
        }
    }

    SilicaFlickable {
        id: mainFlickable
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: mainPageHeader.height
        contentHeight: mainPageHeader.height
        flickableDirection: Flickable.VerticalFlick

        TabHeader {
            id: mainPageHeader
            listView: swipeView
            iconArray: [ /*"image://theme/icon-m-events", "qrc:///images/icon-m-playlists.svg",*/ "image://theme/icon-m-media-songs", "image://theme/icon-m-developer-mode", ]
        }

        PushUpMenu {
            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
        }
    }


    MprisPlayer {
        id: mprisPlayer

        serviceName: "musikilo"
//        property string artist: mediaPlayer.metaData.author!== undefined ?
//                                    mediaPlayer.metaData.author :
//                                    mediaPlayer.metaData.albumArtist !== undefined ?
//                                        mediaPlayer.metaData.albumArtist :
//                                        mediaPlayer.metaData.contributingArtist !== undefined ?
//                                            mediaPlayer.metaData.contributingArtist : ""
//        property string song: mediaPlayer.metaData.title !== undefined ?
//                                  mediaPlayer.metaData.title : ""

//        onArtistChanged: {
//            var metadata = mprisPlayer.metadata

//            metadata[Mpris.metadataToString(Mpris.Artist)] = [artist] // List of strings

//            mprisPlayer.metadata = metadata
//        }

//        onSongChanged: {
//            var metadata = mprisPlayer.metadata

//            metadata[Mpris.metadataToString(Mpris.Title)] = song // String

//            mprisPlayer.metadata = metadata
//        }

        identity: "microtube"

        canControl: true

        canGoNext: true
        canGoPrevious: true
        canPause: true
        canPlay: true
        canSeek: false

//        playbackStatus: mediaPlayer.playbackState === MediaPlayer.PlayingState ?
//                            Mpris.Playing : mediaPlayer.playbackState === MediaPlayer.PausedState ?
//                                Mpris.Paused : Mpris.Stopped

        loopStatus: Mpris.None
        shuffle: false
        volume: 1

        onPauseRequested: {
            playlistmodel.pause()
        }

        onPlayRequested: {
            playlistmodel.resume()
        }

        onPlayPauseRequested: {
            mediaPlayer.playbackState == MediaPlayer.PlayingState ? playlistmodel.pause() : playlistmodel.resume()
        }

        onStopRequested: {
            playlistmodel.pause()
        }

        onNextRequested: {
            if (playlistmodel.activeItem + 1 < playlistmodel.rowCount()) playlistmodel.activeItem++
        }

        onPreviousRequested: {
            if (playlistmodel.activeItem > 0) playlistmodel.activeItem--
        }

        onSeekRequested: {
        }
    }

    CoverActionList {
        id: coverAction

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: if (playlistmodel.activeItem + 1 < playlistmodel.rowCount()) playlistmodel.activeItem++
        }

        CoverAction {
//            iconSource: mediaPlayer.playbackState == MediaPlayer.PlayingState ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
//            onTriggered: mediaPlayer.playbackState == MediaPlayer.PlayingState ? playlistmodel.pause() : playlistmodel.resume()
        }
    }
}
