import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import eu.tgcm 1.0
import "."

Rectangle {
    id:page

    Settings {
        id: settings
    }

    Flickable {
        clip:true
        contentWidth: width
        contentHeight: Math.max(toto.childrenRect.height + 10, height)
        flickableDirection: Flickable.VerticalFlick
        interactive: height < contentHeight
        anchors.fill: parent

        Column {
            id:toto
            spacing:10
            anchors.fill: parent
            Text {
                text: qsTr("Settings")
                anchors.horizontalCenter: parent.horizontalCenter
                font.bold: true
                font.pixelSize: theme.fontSizeSmall
                color:theme.primaryColor
            }

            Text {
                text: qsTr("Server name")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: theme.horizontalPageMargin
                anchors.rightMargin: theme.horizontalPageMargin
                font.pixelSize: theme.fontSizeSmall
                color:theme.primaryColor
            }
            TextField {
                id:serverName
                text:settings.servers[settings.currentServerIdx].name
                anchors.left: parent.left;
                anchors.right: parent.right
                anchors.leftMargin: theme.horizontalPageMargin
                anchors.rightMargin: theme.horizontalPageMargin
                font.pixelSize: theme.fontSizeSmall;
                style: TextFieldStyle {
                    textColor: theme.primaryColor
                    background: Rectangle {
                        border.color: Styling.borderColor
                        anchors.fill: parent
                        radius:5 * scalingFactor
                    }
                }
            }

            Text {
                text:qsTr("Server address")
                anchors.left: parent.left
                anchors.leftMargin: theme.horizontalPageMargin
                color:theme.primaryColor
                font.pixelSize: theme.fontSizeSmall
                id:serverText
            }
            TextField {
                id: serverAddress
                text:settings.servers[settings.currentServerIdx].serverAddress
                anchors.left: parent.left;
                anchors.right: parent.right
                anchors.leftMargin: theme.horizontalPageMargin
                anchors.rightMargin: theme.horizontalPageMargin
                inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
                font.pixelSize: theme.fontSizeSmall;
                style: TextFieldStyle {
                    textColor: theme.primaryColor
                    background: Rectangle {
                        border.color: Styling.borderColor
                        anchors.fill: parent
                        radius:5 * scalingFactor
                    }
                }
            }

            Text {
                id : serverPortText
                text:qsTr("Server Port")
                anchors.left: parent.left
                anchors.leftMargin: theme.horizontalPageMargin
                color:theme.primaryColor
                font.pixelSize: theme.fontSizeSmall
            }
            TextField {
                id: serverPort
                text:settings.servers[settings.currentServerIdx].serverPort
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: theme.horizontalPageMargin
                anchors.rightMargin: theme.horizontalPageMargin
                validator: IntValidator { bottom: 1; top: 65535 }
                font.pixelSize: theme.fontSizeSmall;
                inputMethodHints: Qt.ImhDigitsOnly
                style: TextFieldStyle {
                    textColor: Styling.linkColor
                    background: Rectangle {
                        border.color: Styling.borderColor
                        anchors.fill: parent
                        radius: 5
                    }
                }
            }

            Text {
                id : serverHttpPortText
                text:qsTr("Web Port")
                anchors.left: parent.left
                anchors.leftMargin: theme.horizontalPageMargin
                color:theme.primaryColor
                font.pixelSize: theme.fontSizeSmall
            }
            TextField {
                id: serverHttpPort
                text:settings.servers[settings.currentServerIdx].serverHttpPort
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: theme.horizontalPageMargin
                anchors.rightMargin: theme.horizontalPageMargin
                validator: IntValidator { bottom: 1; top: 65535 }
                font.pixelSize: theme.fontSizeSmall;
                inputMethodHints: Qt.ImhDigitsOnly
                style: TextFieldStyle {
                    textColor: Styling.linkColor
                    background: Rectangle {
                        border.color: Styling.borderColor
                        anchors.fill: parent
                        radius: 5
                    }
                }
            }

            Text {
                id:labelDpi
                text: qsTr("Dpi")
                anchors.left: parent.left
                anchors.leftMargin:  theme.horizontalPageMargin
                color:theme.primaryColor
                font.pixelSize: theme.fontSizeSmall
            }

            TextField {
                id: valueDpi
                text:settings.dpi
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: theme.horizontalPageMargin
                anchors.rightMargin: theme.horizontalPageMargin
                validator: IntValidator { bottom: 25; top: 65535 }
                font.pixelSize: theme.fontSizeSmall;
                inputMethodHints: Qt.ImhDigitsOnly
                style: TextFieldStyle {
                    textColor: Styling.linkColor
                    background: Rectangle {
                        border.color: Styling.borderColor
                        anchors.fill: parent
                        radius: 5
                    }
                }
            }

            Button {
                text: "Save"
                anchors.right: parent.right
                anchors.rightMargin: 2 * theme.horizontalPageMargin
                onClicked: {
                    settings.servers[settings.currentServerIdx].setServerAddress(serverAddress.text);
                    settings.servers[settings.currentServerIdx].setServerPort(serverPort.text);
                    settings.servers[settings.currentServerIdx].setServerHttpPort(serverHttpPort.text);
                    settings.servers[settings.currentServerIdx].setName(serverName.text)
                    //settings.setIgnoreWifiStatus(chkIgnoreWifi.checked);
                    settings.setDpi(valueDpi.text)
                    settings.save();
                    deviceInformation.update()
                }
            }

        }
    }
}
