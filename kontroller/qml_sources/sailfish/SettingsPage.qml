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
                model: appSettings.servers
                delegate: ListItem {
                    id: listItem
                    DetailItem {
                        label: model.name
                        value: model.serverAddress
                        anchors.verticalCenter: parent.verticalCenter
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
                    appSettings.downloadLocationIndex = currentIndex
                }
                menu: ContextMenu
                {
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
                id:chkIgnoreWifi
                text:qsTr("Ignore wi-fi status")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
                onCheckedChanged:
                {
                    appSettings.ignoreWifiStatus = checked;
                }
                Component.onCompleted: {
                    checked = appSettings.ignoreWifiStatus
                }
            }
        }
    }

    property var serverSettingsComponent

    Component.onCompleted: {
        serverSettingsComponent = Qt.createComponent(Qt.resolvedUrl("ServerSettingsPage.qml"))
    }

    function pushServerSettingsPage(serveruuid)
    {
        console.log("push server settings for server " + serveruuid);
        if(serverSettingsComponent.status === Component.Error)
        console.log(serverSettingsComponent.errorString())
        pageStack.push(serverSettingsComponent.createObject(pageStack,
                                                            {"serverUuid": serveruuid}))
    }

    function addNewServer()
    {
        var newserveruuid = appSettings.newServer().uuid; // will give a uuid
        pushServerSettingsPage(newserveruuid); // go to settings page
    }

    function deleteServer(uuid)
    {
        appSettings.deleteServer(uuid)
    }

    onStatusChanged: {
        if(status === PageStatus.Deactivating)
        {
            appSettings.save(); // always save on deactivating the page
        }
    }
}
