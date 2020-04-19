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
                text: qsTr("<b>Kontroller</b> is released under GNU General Public License version 3, in the hope that it will\
 be usefull. The following persons contributed to Kontroller, either via code or translations:")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.rightMargin: Theme.horizontalPageMargin
            }
            Repeater {
                model: ["Julien Blanc", "Åke Engelbrektson", "Вячеслав Диконов"]
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
                text: qsTr("Kontroller uses QJsonRPC. Many thanks to all the people who contributed to this project.")
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
                text: qsTr("Kontroller home page:")
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
        }

    }

}
