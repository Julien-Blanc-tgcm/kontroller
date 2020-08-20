import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0

Item {
    id: settingsPage

    implicitHeight: swipeView.height; implicitWidth: swipeView.width

    Connections {
        target: swipeView
        onCurrentIndexChanged: {
            appSettings.save();
        }
    }

    SilicaFlickable {
        id: flickable
        anchors.fill: parent

        PageHeader {
            id: header
            title: qsTr("Settings")
        }

        PullDownMenu {
            MenuItem {
                text: qsTr("Add server")
                onClicked: pageStack.push(Qt.resolvedUrl("ServerSettingsPage.qml", {serverUuid: appSettings.newServer().uuid}))
            }
        }

        Column {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: header.bottom
            anchors.bottom: parent.bottom

            SectionHeader {
                id: servers
                text: qsTr("Servers")
            }

            SilicaListView {
                anchors.left: parent.left
                anchors.right: parent.right
                height: contentHeight
                model: appSettings.servers
                delegate: ListItem {
                    id: listItem

                    DetailItem {
                        anchors.verticalCenter: parent.verticalCenter
                        label: name
                        value: appClient.server ? (appClient.server.name === name ? qsTr("Connected") : qsTr("Disconnected")) : qsTr("Disconnected")
                    }

                    RemorseItem {
                        id: remorse
                    }

                    onClicked: {
                        appClient.switchToServer(modelData.uuid)
                        appSettings.lastServer = modelData.uuid
                    }

                    function showRemorseItem() {
                        remorse.execute(listItem, qsTr("Removed"), function() { appSettings.deleteServer(uuid) });
                    }

                    menu: ContextMenu {
                        MenuItem {
                            text: qsTr("Edit")
                            onClicked: pageStack.push(Qt.resolvedUrl("ServerSettingsPage.qml"), {serverUuid: uuid})
                        }
                        MenuItem {
                            text: qsTr("Remove")
                            onClicked: showRemorseItem()
                        }
                    }
                }
            }

            SectionHeader {
                text: qsTr("Common")
            }

            ComboBox {
                label: qsTr("Download location")
                description: qsTr("Where to put downloaded files. By default, use \
the phone memory, but SD card memory can be used as well. The relevant folder will \
be used, depending on the downoaded file type.")

                onCurrentItemChanged: appSettings.downloadLocationIndex = currentIndex

                menu: ContextMenu {
                    Repeater {
                        model: appSettings.possibleDownloadLocations
                        delegate: MenuItem {
                            text: (model.modelData.typeAsInt === DownloadLocation.Phone) ?
                                      qsTr("Phone memory") :
                                      qsTr("SD Card %1").arg(model.modelData.name);
                        }
                    }
                }

                Component.onCompleted: {
                    currentIndex = appSettings.downloadLocationIndex;
                }
            }

            TextSwitch {
                text: qsTr("Ignore wi-fi status")
                anchors.left: parent.left
                anchors.right: parent.right

                onCheckedChanged: appSettings.ignoreWifiStatus = checked

                Component.onCompleted: checked = appSettings.ignoreWifiStatus
            }
        }
    }
}
