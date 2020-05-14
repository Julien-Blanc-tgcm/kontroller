import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    property var volumePlugin
    property bool updated: false
    implicitHeight: volumeSlider.height
    Slider {
        value: volumePlugin?volumePlugin.currentVolume:0
        minimumValue: volumePlugin?volumePlugin.minVolume:0
        maximumValue: volumePlugin?volumePlugin.maxVolume:1
        stepSize: 1
        onReleased: {
            if(volumePlugin)
                volumePlugin.updateVolume(value);
            else
                console.log("volumePlugin is null !!!");
        }
        width: parent.width
        valueText: volumePlugin?qsTr("Volume: %1").arg(volumePlugin.formatVolume(value)):qsTr("No volume");
        id: volumeSlider
    }
    onVolumePluginChanged: {
        if(volumePlugin)
        {
            // this part is needed, because the change of the value property by the slider breaks the automatic update
            // of the value (no longer connected), so we put it back via this signal handler
            volumePlugin.onCurrentVolumeChanged.connect(function() {
                volumeSlider.value = volumePlugin.currentVolume;
            });
            updated = true;
        }
    }
}
