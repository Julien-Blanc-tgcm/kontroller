import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import eu.tgcm 1.0

Flickable {
    KodiSettings {
        id: settings
    }
    clip:true
    contentWidth: width
    contentHeight: Math.max(toto.childrenRect.height + 10 * scalingFactor, height)
    flickableDirection: Flickable.VerticalFlick
    interactive: height < contentHeight
    Rectangle {
        id:toto;
        anchors.fill: parent
        color:"#000";
        Text {
            text:qsTr("Server")
            anchors.verticalCenter: serverAddress.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5 * scalingFactor
            color:appstyle.textColor
            font.bold: true
            font.pixelSize: 12 * scalingFactor
            id:serverText
        }
        TextField {
            id: serverAddress
            text:settings.serverAddress
            anchors.top:parent.top
            anchors.topMargin: 5 * scalingFactor
            anchors.left: parent.left;
            anchors.leftMargin: {
                var x = Math.max(serverText.width, serverPortText.width) + 10 * scalingFactor
                if(x < parent.width / 4)
                    return x + 30 * scalingFactor;
                else
                    return x + 5 * scalingFactor;
            }
            anchors.right: parent.right
            anchors.rightMargin: 10
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
            font.pixelSize: 12 * scalingFactor;
            height:26 * scalingFactor
            style: TextFieldStyle {
                textColor: appstyle.textColor
                background: Rectangle {
                    color:"#000";
                    border.color: appstyle.borderColor
                    anchors.fill: parent
                    radius:5 * scalingFactor
                }
            }
        }
        Text {
            id : serverPortText
            text:qsTr("Port")
            anchors.left: parent.left
            anchors.leftMargin: 5 * scalingFactor
            anchors.verticalCenter: serverPort.verticalCenter
            color:appstyle.textColor
            font.bold: true
            font.pixelSize: 12 * scalingFactor
        }
        TextField {
            id: serverPort
            text:settings.serverPort
            anchors.top:serverAddress.bottom
            anchors.topMargin: 10 * touchScalingFactor;
            anchors.left: serverAddress.left

            anchors.right: parent.right
            anchors.rightMargin: 5 * scalingFactor
            validator: IntValidator { bottom: 1; top: 65535 }
            font.pixelSize: 12 * scalingFactor;
            inputMethodHints: Qt.ImhDigitsOnly
            height: 26*scalingFactor;
            style: TextFieldStyle {
                textColor: appstyle.textColor
                background: Rectangle {
                    color:"#000";
                    border.color: appstyle.borderColor
                    anchors.fill: parent
                    radius: 5 * scalingFactor
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
            anchors.topMargin: 10 * touchScalingFactor
            style: CheckBoxStyle {
                label: Label {
                    color: appstyle.textColor
                    text: qsTr("Use HTTP transport")
                    font.pixelSize: 12 * scalingFactor
                }
                indicator: Rectangle {
                    implicitHeight: 16 * scalingFactor
                    implicitWidth: 16 * scalingFactor
                    radius : 3 * scalingFactor
                    Rectangle {
                        visible: control.checked
                        color: "#555"
                        border.color: "#333"
                        radius: 1
                        anchors.margins: 3 * scalingFactor
                        anchors.fill: parent
                    }
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
            anchors.top:chkUseHttpInterface.bottom
            anchors.topMargin: 10 * touchScalingFactor
            style: CheckBoxStyle {
                label: Label {
                    color: appstyle.textColor
                    text: qsTr("Use file browsing for music")
                    font.pixelSize: 12 * scalingFactor
                }
                indicator: Rectangle {
                    implicitHeight: 16 * scalingFactor
                    implicitWidth: 16 * scalingFactor
                    radius : 3 * scalingFactor
                    Rectangle {
                        visible: control.checked
                        color: "#555"
                        border.color: "#333"
                        radius: 1
                        anchors.margins: 3 * scalingFactor
                        anchors.fill: parent
                    }
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
            anchors.top:chkMusicFileBrowsing.bottom
            anchors.topMargin: 10 * touchScalingFactor
            style: CheckBoxStyle {
                label: Label {
                    color: appstyle.textColor
                    text: qsTr("Use file browsing for videos")
                    font.pixelSize: 12 * scalingFactor
                }
                indicator: Rectangle {
                    implicitHeight: 16 * scalingFactor
                    implicitWidth: 16 * scalingFactor
                    radius : 3 * scalingFactor
                    Rectangle {
                        visible: control.checked
                        color: "#555"
                        border.color: "#333"
                        radius: 1
                        anchors.margins: 3 * scalingFactor
                        anchors.fill: parent
                    }
                }
            }
        }
        Text {
            id:labelDpi
            text: qsTr("Dpi")
            anchors.left: serverText.left
            color:appstyle.textColor
            anchors.verticalCenter:valueDpi.verticalCenter
            font.bold: true
            font.pixelSize: 12 * scalingFactor
        }

        TextField {
            id: valueDpi
            text:settings.dpi
            anchors.top:chkVideosFileBrowsing.bottom
            anchors.topMargin: 10 * touchScalingFactor;
            anchors.left: serverAddress.left
            anchors.right: parent.right
            anchors.rightMargin: 5 * scalingFactor
            validator: IntValidator { bottom: 25; top: 65535 }
            font.pixelSize: 12 * scalingFactor;
            inputMethodHints: Qt.ImhDigitsOnly
            height: 26*scalingFactor;
            style: TextFieldStyle {
                textColor: appstyle.textColor
                background: Rectangle {
                    color:"#000";
                    border.color: appstyle.borderColor
                    anchors.fill: parent
                    radius: 5 * scalingFactor
                }
            }
        }

        Button {
            text: "Save"
            anchors.top:valueDpi.bottom
            anchors.topMargin: 10 * scalingFactor
            anchors.right: parent.right
            anchors.rightMargin: 10 * scalingFactor
            onClicked: {
                // settings.musicFileBrowsing = chkMusicFileBrowsing.checked;
                //settings.videosFileBrowsing = chkVideosFileBrowsing.checked;
                settings.setServer(serverAddress.text, serverPort.text)
                deviceInformation.update()
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
        Binding {
            target:settings
            property: "dpi"
            value: valueDpi.text
        }
    }
}
