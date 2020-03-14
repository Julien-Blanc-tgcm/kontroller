import QtQuick 2.0

import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0
import "."

Page {
    id:page

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: theCol.height

        Column {
            id:theCol
            spacing:10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top:parent.top

            PageHeader {
                title: qsTr("Settings")
            }
            SectionHeader {
                text: qsTr("Servers")
                id: endOfHeaders
            }

            SilicaListView {
                anchors.left: parent.left
                anchors.right: parent.right
                height: contentHeight
                model: settings.servers
                delegate: ListItem {
                    id: listItem
                    DetailItem {
                        label: model.name
                        value: model.serverAddress
                    }
                    RemorseItem {
                        id: remorse
                    }

                    onClicked: pushServerSettingsPage(model.uuid)
                    function showRemorseItem() {
                        var uuid = model.uuid;
                        remorse.execute(listItem, qsTr("Deleted"), function() { deleteServer(uuid) });
                    }

                    menu: ContextMenu {
                        MenuItem
                        {
                            text: qsTr("Delete server")
                            onClicked: showRemorseItem()
                        }
                    }
                }
            }

            Item {
                height: row.height
                anchors.left: parent.left
                anchors.right: parent.right
                Row {
                    id:row
                    Icon {
                    source:"image://theme/icon-m-add"
                    }
                    Label {
                        text: qsTr("Add server")
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                MouseArea {
                    anchors.fill:parent
                    onClicked: addNewServer()
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
                onCurrentItemChanged: {
                    settings.updateDownloadFolder(currentIndex)
                }
                menu: ContextMenu
                {
                    Repeater {
                        model: settings.possibleDownloadFolders
                        delegate: MenuItem {
                            text: (model.typeAsInt === DownloadLocation.Phone) ?
                                      qsTr("Phone memory") :
                                      qsTr("SD Card %1").arg(model.name);
                        }
                    }
                }
                Component.onCompleted: {
                    currentIndex = (settings.downloadFolder === StandardPaths.music) ? 0 : 1;
                }
            }

            TextSwitch {
                id:chkIgnoreWifi
                checked: settings.ignoreWifiStatus
                text:qsTr("Ignore wi-fi status")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                onTextChanged:
                {
                    settings.setIgnoreWifiStatus(chkIgnoreWifi.checked);
                    settings.save();
                }
            }
        }
    }

    property var zones;

    property var serverSettingsComponent

    Component.onCompleted: {
        pushOrPullZonePage();
        serverSettingsComponent = Qt.createComponent(Qt.resolvedUrl("ServerSettingsPage.qml"))
    }

    function pushOrPullZonePage()
    {
        console.log("pushhere");
        if(chkIgnoreWifi.checked)
        {
            console.log("pushAttached");
            zones = pageStack.pushAttached(Qt.resolvedUrl("ManageZones.qml"));
        }
        else if(zones)
            pageStack.popAttached(zones);
    }

    function pushServerSettingsPage(serveruuid)
    {
        console.log("push server settings for server " + serveruuid);
        pageStack.push(serverSettingsComponent.createObject(pageStack,
                                                            {"serverUuid": serveruuid}))
    }

    function addNewServer()
    {
        var newserveruuid = settings.newServer(); // will give a uuid
        pushServerSettingsPage(newserveruuid); // go to settings page
    }

    function deleteServer(uuid)
    {
        settings.deleteServer(uuid)
    }
}
