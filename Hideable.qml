import QtQuick 2.0

Rectangle {
    id:main
    property var anchorFlicked;
    property var anchorRaised;
    states :[
        State {
            name: "lowered"
            AnchorChanges {
                target: main
                anchors.top:anchorFlicked
            }
            PropertyChanges {
                target:main
                visible:false
                focus:false
            }
        },
        State {
            name: "raised"
            AnchorChanges {
                target:main
                anchors.top:anchorRaised
            }
            PropertyChanges {
                target: main
                visible: true
                focus:true
            }
        }
    ]
    state:"lowered"
    transitions: [Transition {
            from: "lowered"
            to: "raised"
            SequentialAnimation {
                PropertyAnimation {
                    property:"visible"
                    duration: 1
                }
                AnchorAnimation {
                    duration: 500
                }
            }
            reversible: true
        }

    ]

    Keys.onReleased: {
        if(event.key === Qt.Key_Back || event.key === Qt.Key_Backspace)
        {
            state = "lowered"
            event.accepted = true
        }
    }
}

