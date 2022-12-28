import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.eu.tgcm 1.0

Column {
    property var settings

    property var parameters : {
        "address": undefined,
        "updateSource": undefined,
        "source": undefined,
        "forceKodiVolume": undefined,
        "kodiForcedVolume": undefined
    }

    Label {
        text: qsTr("AVR Plugin parameters")
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: Theme.horizontalPageMargin
        wrapMode: Text.WordWrap
        color: Theme.highlightColor
    }

    TextField {
        id: avrAddress
        text: parameters.address
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: Theme.horizontalPageMargin
        inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhPreferNumbers
        label:qsTr("A/V Receiver IP address")
        placeholderText: qsTr("A/V Receiver IP address")
        EnterKey.iconSource: "image://theme/icon-m-enter-next"
        EnterKey.onClicked: avrSource.focus = false
        onTextChanged: updateParameters()
    }

    TextSwitch {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: Theme.horizontalPageMargin
        text: qsTr("Automatically switch source");
        id: avrSourceSwitch
        checked: parameters.updateSource
        onCheckedChanged: updateParameters()
    }

    ComboBox {
        id: avrSource
        visible: avrSourceSwitch.checked
        width: parent.width
        menu: ContextMenu {
            Repeater {
                model: allSources__
                delegate: MenuItem {
                    text: model.modelData
                }
            }
        }
        onCurrentIndexChanged: updateParameters()
        label: qsTr("Source")
    }

    TextSwitch {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: Theme.horizontalPageMargin
        text: qsTr("Force kodi volume to a given level");
        id: avrForceKodiVolume
        checked: parameters.forceKodiVolume
        onCheckedChanged: updateParameters()
    }

    TextField {
        id: avrKodiForcedVolume
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.horizontalPageMargin
        anchors.rightMargin: Theme.horizontalPageMargin
        label: qsTr("Volume to force")
        placeholderText: "100"
        text: parameters.kodiForcedVolume
        validator: IntValidator{ bottom:1; top:100}
        visible: avrForceKodiVolume.checked
        onTextChanged: updateParameters()
    }

    onSettingsChanged: function() {
        console.log("Settings changed !!!")
        if (settings)
        {
            if (settings.volumePluginParameters.address !== undefined)
                avrAddress.text = settings.volumePluginParameters.address
            if (settings.volumePluginParameters.updateSource !== undefined)
                avrSourceSwitch.checked = settings.volumePluginParameters.updateSource
            if (settings.volumePluginParameters.source !== undefined)
            {
                for (var i = 0; i < allSources__.length; ++i)
                {
                    if (settings.volumePluginParameters.source === allSources__[i])
                        avrSource.currentIndex = i
                }
            }
            if (settings.volumePluginParameters.forceKodiVolume !== undefined)
                avrForceKodiVolume.checked = settings.volumePluginParameters.forceKodiVolume
            if (settings.volumePluginParameters.kodiForcedVolume !== undefined)
                avrKodiForcedVolume.text = settings.volumePluginParameters.kodiForcedVolume
        }
    }

    function updateParameters()
    {
        parameters.address = avrAddress.text
        parameters.updateSource = avrSourceSwitch.checked
        if (avrSource.currentItem)
            parameters.source = avrSource.currentItem.text
        parameters.forceKodiVolume = avrForceKodiVolume.checked
        parameters.kodiForcedVolume = Number(avrKodiForcedVolume.text)
    }

    property var allSources__: [
        "Phono",
        "CD",
        "DVD",
        "Bluray",
        "TV",
        "Cable_Sat",
        "Multimedia",
        "Game",
        "Tuner",
        "HdRadio",
        "Aux1",
        "Aux2",
        "Aux3",
        "Aux4",
        "Aux5",
        "Aux6",
        "Aux7",
        "Network",
        "Bluetooth"
    ]
}
