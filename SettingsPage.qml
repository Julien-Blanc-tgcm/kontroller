import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import eu.tgcm 1.0

Rectangle {
    anchors.fill: parent;
    KodiSettings {
        id: settings
    }

    color:"#000";
    Text {
        text:qsTr("Server")
        anchors.verticalCenter: serverAddress.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        color:"#00c"
        font.bold: true
    }
    TextField {
        id: serverAddress
        text:settings.serverAddress
        anchors.top:parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 100
        anchors.right: parent.right
        anchors.rightMargin: 10
        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
        style: TextFieldStyle {
            textColor: "#00c"
            background: Rectangle {
                color:"#000";
                border.color: "#00a"
                anchors.fill: parent
                radius:10
                implicitHeight: 40
            }
        }
    }
    Text {
        text:qsTr("Port")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: serverPort.verticalCenter
        color:"#00c"
        font.bold: true
    }
    TextField {
        id: serverPort
        text:settings.serverPort
        anchors.top:serverAddress.bottom
        anchors.topMargin: 10;
        anchors.left: parent.left
        anchors.leftMargin: 100
        anchors.right: parent.right
        anchors.rightMargin: 10
        validator: IntValidator { bottom: 1; top: 65535 }
        inputMethodHints: Qt.ImhDigitsOnly
        style: TextFieldStyle {
            textColor: "#00c"
            background: Rectangle {
                color:"#000";
                border.color: "#00a"
                anchors.fill: parent
                radius:10
                implicitHeight: 40
            }
        }
    }
    CheckBox {
        id: chkUseHttpInterface
        checked:settings.useHttpInterface
        text: qsTr("Use HTTP transport")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.top:serverPort.bottom
        anchors.topMargin: 10
        style: CheckBoxStyle {
            label: Label {
                color: "#00f"
                text: qsTr("Use HTTP transport")
            }
        }
    }

    CheckBox {
        id : chkMusicFileBrowsing
        checked: settings.musicFileBrowsing
        text: qsTr("Use file browsing for music")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.top:parent.top
        anchors.topMargin: 200
        style: CheckBoxStyle {
            label: Label {
                color: "#00f"
                text: qsTr("Use file browsing for music")
            }
        }
    }
    CheckBox {
        id: chkVideosFileBrowsing
        checked:settings.videosFileBrowsing
        text: qsTr("Use file browsing for videos")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.top:parent.top
        anchors.topMargin: 280
        style: CheckBoxStyle {
            label: Label {
                color: "#00f"
                text: qsTr("Use file browsing for videos")
            }
        }
    }

    Button {
        text: "Save"
        x:50
        anchors.top:parent.top
        anchors.topMargin: 440
        onClicked: {
           // settings.musicFileBrowsing = chkMusicFileBrowsing.checked;
            //settings.videosFileBrowsing = chkVideosFileBrowsing.checked;
            settings.setServer(serverAddress.text, serverPort.text)
        }
    }


    Binding {
        target: settings
        property: "musicFileBrowsing"
        value: chkMusicFileBrowsing.checked;
    }
    Binding {
        target:settings
        property: "serverPort"
        value:serverPort.text
    }
    Binding{
        target:settings
        property: "videosFileBrowsing"
        value: chkVideosFileBrowsing.checked
    }
    Binding {
        target: settings
        property: "serverAddress"
        value: serverAddress.text
    }
    Binding {
        target: settings
        property: "useHttpInterface"
        value: chkUseHttpInterface.checked
    }
}
