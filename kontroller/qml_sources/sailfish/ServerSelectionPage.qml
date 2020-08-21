import QtQuick 2.6

import Sailfish.Silica 1.0
import harbour.eu.tgcm 1.0
import "."

Page {
    id:page

    SilicaFlickable {
        anchors.fill: parent

        SilicaListView {
            header: PageHeader {
                title: qsTr("Server selection")
            }

            anchors.left: parent.left
            anchors.right: parent.right
            height: contentHeight
            model: appSettings.servers
            delegate: ListItem {
                id: listItem
                contentHeight: Theme.itemSizeMedium
                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: Theme.horizontalPageMargin
                    anchors.right: parent.right
                    anchors.rightMargin: Theme.horizontalPageMargin
                    text: model.name
/*                        value: model.serverAddress */
                    anchors.verticalCenter: parent.verticalCenter
                }

                onClicked: {
                    appClient.switchToServer(model.uuid);
                    appSettings.lastServer = model.uuid;
                    pageStack.pop();
                }
            }
        }
    }

}
