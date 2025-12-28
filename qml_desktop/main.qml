import QtQuick 2.0
import QtQuick.Controls 1.0
import eu.tgcm 1.0

ApplicationWindow
{
    visible: true
    width: 1280
    height: 720

    KodiServiceDiscovery {
        id: serviceDiscovery
    }

    Column {
        ComboBox {
            id: theCombo
            model: appSettings.servers
            textRole: "name"
            onCurrentIndexChanged: {
                console.log("Will switch to " + model[currentIndex].uuid);
                appClient.switchToServer(model[currentIndex].uuid)
            }
        }
        Button {
            text: qsTr("Start discovery")
            onClicked: serviceDiscovery.startDiscovery()
        }
        Label {
            text: qsTr("Connection status: %1").arg(appClient.connectionStatus)
        }

        Repeater {
            model: serviceDiscovery.servers
            delegate: Label {
                text: qsTr("Host %1, address %2").arg(model.modelData.hostname).arg(model.modelData.address)
                MouseArea {
                    anchors.fill: parent
                    onClicked: createServer(model.modelData)
                }
            }
        }
    }
    Component.onCompleted: {
        theCombo.model = appSettings.servers
        console.log("Hey he hey: " + appSettings.servers.length)
        console.log(appSettings)
        console.log(appClient)
    }

    function createServer(data)
    {
        console.log("Saving data")
        var server = appSettings.newServer();
        server.setName(data.hostname);
        server.setServerAddress(data.address);
        server.setServerHttpPort(8080);
        server.setServerPort(9090);
        appSettings.save();
    }
}
