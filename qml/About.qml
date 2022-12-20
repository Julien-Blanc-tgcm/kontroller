import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.childrenRect.height
        Column {
            id: column
            anchors.fill: parent
            spacing: Theme.paddingMedium
            PageHeader {
                title: qsTr("About");
            }

            Label {
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
                text: qsTr("<b>Kontroller</b> is copylefted libre software, licensed GPLV3+.\
Brought to you by these people:")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
            }
            Repeater {
                model: ["Julien Blanc", "Åke Engelbrektson", "Вячеслав Диконов", "popanz", "luj", "nthn", "comradekingu"]
                Label {
                    color:Theme.highlightColor
                    wrapMode: Text.WordWrap
                    text: model.modelData
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: Theme.horizontalPageMargin * 2
                    anchors.rightMargin: Theme.horizontalPageMargin
                }
            }
            Separator {
                width: parent.width / 2
                horizontalAlignment: Qt.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
                text: qsTr("Kontroller uses QJsonRPC. Many thanks to its contributors.")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
            }
            Separator {
                width: parent.width / 2
                horizontalAlignment: Qt.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
                text: qsTr("Kontroller homepage:")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
            }
            LinkedLabel {
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
                plainText: "https://github.com/Julien-Blanc-tgcm/kontroller/"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
            }
            Separator {
                width: parent.width / 2
                horizontalAlignment: Qt.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
                text: qsTr("Version %1").arg("0.24")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
            }
        }

    }

}
