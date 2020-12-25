import QtQuick 2.0
import Sailfish.Silica 1.0

Button {

    property string labelPlay:""
    property string labelResume:""
    property bool offerResume: false

    property bool modeResume_: offerResume
    property bool insideSwitchMode_: false

    text:modeResume_?labelResume:labelPlay
    onClicked: play_()
    onPressAndHold: {
        modeResume_ = false;
        insideSwitchMode_ = true;
    }
    onCanceled: {
        modeResume_ = offerResume;
        insideSwitchMode_ = false;
    }
    onReleased: {
        if(insideSwitchMode_)
            play();
        insideSwitchMode_ = false;
        modeResume_ = offerResume;
    }
    function play_()
    {
        if(modeResume_)
            resume()
        else
            play()
    }

    signal resume
    signal play
}
