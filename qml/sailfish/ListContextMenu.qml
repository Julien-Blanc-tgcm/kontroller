import QtQuick 2.0
import "."
Rectangle {
    states : [
        State {
            name: "album"
            PropertyChanges {
                target:main
                contextMenuModel: [
                { text:qsTr("Add to playlist"),
                        type:"addtoplaylist"},
                    {text : qsTr("Play immediately"),
                        type: "play"},
                    {text: qsTr("View information"),
                        type:"information"}
                ]
            }
        },
        State {
            name: "video"
            PropertyChanges {
                target:main
                contextMenuModel: [
                    { text:qsTr("Add to playlist"), type:"addtoplaylist"},
                    { text:qsTr("Play immediately"), type:"play"},
                    { text:qsTr("View information"), type:"information"}
                ]
            }
        },
        State {
            name : "image"
        },
        State {
            name : "artist"
            PropertyChanges {
                target: main
                contextMenuModel: [
                    { text:qsTr("View information"), type:"information"}
                ]
            }
        },
        State {
            name: "song"
            PropertyChanges {
                target: main
                contextMenuModel: [
                    { text:qsTr("Add to playlist"), type:"addtoplaylist"},
                    { text:qsTr("Play immediately"), type:"play"}
                ]
            }
        },
        State {
            name: "audiodirectory"
            PropertyChanges {
                target: main
                contextMenuModel: [
                    { text:qsTr("Add to playlist"), type:"addtoplaylist"},
                    { text:qsTr("Play immediately"), type:"play"}
                ]
            }
        },
        State {
            name: "movie"
            PropertyChanges {
                target:main
                contextMenuModel: [
                    {text: qsTr("Add to playlist"), type: "addtoplaylist"},
                    {text:qsTr("Play immediately"), type: "play" },
                    {text: qsTr("View information"), type: "information"}
                ]
            }
        },
        State {
            name:"season"
            PropertyChanges {
                target:main
                contextMenuModel: [
                    {text: qsTr("View information"), type:"information" },
                    {text: qsTr("Play immediately"), type:"play"},
                    {text: qsTr("Add to playlist"), type:"addtoplaylist"}
                ]
            }
        },
        State {
            name:"episode"
            PropertyChanges {
                target: main
                contextMenuModel: [
                    {text: qsTr("View information"), type:"information"},
                    {text: qsTr("Play immediately"), type:"play"},
                    {text: qsTr("Add to playlist"), type:"addtoplaylist"}
                ]

            }
        }

    ]
    id:main
    //property string color;
    //property alias border : genericMenu.border;
    state: ""
    property double elementHeight : 20

    property var contextMenuModel : [];

    height: genericMenu.height + 10
    width: genericMenu.width + 10
    Column {
        //height : contextMenuModel.length * elementHeight
        id: genericMenu;
        visible:true
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top:parent.top
        anchors.topMargin: 5
        //color:parent.color
        Repeater {
            model:contextMenuModel
            Text {
                height:elementHeight
                verticalAlignment: Text.AlignVCenter
                text:model.modelData.text
                MouseArea {
                    anchors.fill: parent
                    onClicked: dispatchEvent(model.modelData.type)
                }
            }
        }
    }

    function dispatchEvent(type)
    {
        if(type === "information")
        {
            informationPressed();
        }
        else if(type === "play")
            playPressed();
        else if(type === "addtoplaylist")
            addToPlaylistPressed();
    }

    property var currentModel

    function showSubMenu(item, model, x, y) {
        main.currentModel = model
        var coord = main.parent.mapFromItem(item, x, y);
        main.x = coord.x;
        main.y = coord.y;
        if(model.filetype === "directory")
            main.state = "audiodirectory";
        else if(model.filetype === "album")
            main.state = "album";
        else if(model.filetype === "artist")
            main.state = "artist";
        else if(model.filetype === "song" || model.filetype === "file")
            main.state = "song";
        else if(model.filetype === "directory")
            main.state = "directory";
        else if(model.filetype === "season")
            main.state = "season";
        else if(model.filetype === "episode")
            main.state = "episode";
        else
        {
            console.log("No menu defined for " + model.filetype);
        }
        main.visible = true
    }
    onWidthChanged: ensureInVisibleArea()
    onVisibleChanged: ensureInVisibleArea()

    function ensureInVisibleArea()
    {
        if(parent)
        {
            if(x + width > parent.width)
            {
                x = parent.width - width - 5
            }
            if(y + height > parent.height)
                y = parent.height - height
        }
    }

    signal playPressed()
    signal addToPlaylistPressed()
    signal informationPressed()
}

