import QtQuick 2.0
import QtFeedback 5.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."

Page {
    id:page;
    anchors.fill: parent
    Remote {
        id: remoteController
    }
    HapticsEffect
    {
        id: commandFeedback
        intensity: 0.5 // range 0-1
        duration: 100 // ms
    }

    property int buttonWidth : page.width / 5;

    property int runningCount:0

    Timer {
        id:timer;
        interval: 300;
        running: false;
        repeat:true;
        property string curButton : "";
        onTriggered: {
            curButton = "";
            var mouseEndX = mArea.mouseX;
            var mouseEndY = mArea.mouseY;
            var diffX = mouseEndX - mArea.mouseStartX;
            var diffY = mouseEndY - mArea.mouseStartY;
            if(Math.abs(diffX) > Math.abs(diffY))
            {
                if(diffX > 100)
                    curButton = "right";
                else if(diffX < -100)
                    curButton = "left";

                if(Math.abs(diffX) > 300)
                    interval = 100;
                else if(Math.abs(diffX) < 100)
                    interval = 400;
                else
                    interval = 400 - (Math.abs(diffX) / 3 * 4);
            }
            else
            {
                if(diffY > 100)
                    curButton = "down";
                else if(diffY < -100)
                    curButton = "up";
                if(Math.abs(diffY) > 300)
                    interval = 100;
                else if(Math.abs(diffY) < 100)
                    interval = 400;
                else
                    interval = 400 - (Math.abs(diffY) / 3 * 4);
            }
//            console.log(diffX, diffY, curButton);

            if(curButton == "left")
            {
                remoteController.left();
                commandFeedback.start();
            }
            else if(curButton == "right")
            {
                remoteController.right();
                commandFeedback.start();
            }
            else if(curButton == "up")
            {
                remoteController.up();
                commandFeedback.start();
            }
            else if(curButton == "down")
            {
                remoteController.down();
                commandFeedback.start();
            }
        }
    }

    Row {
        height:buttonWidth
        id:theRow
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        spacing: 25
        IconButton {
            width: buttonWidth
            height: buttonWidth
            icon.source: "image://theme/icon-m-back"
            icon.scale: 2
            onClicked:
            {
                remoteController.back()
                commandFeedback.start();
            }
        }
        IconButton {
            width: buttonWidth
            height: buttonWidth
            icon.source: "image://theme/icon-m-home"
            icon.scale: 2
            onClicked:
            {
                remoteController.home()
                commandFeedback.start();
            }
        }
        IconButton {
            width: buttonWidth
            height: buttonWidth
            icon.source: "image://theme/icon-m-question"
            icon.scale: 2
            onClicked:
            {
                remoteController.info()
                commandFeedback.start();
            }
        }
        IconButton{
            width: buttonWidth
            height: buttonWidth
            icon.source: "image://theme/icon-m-menu"
            icon.scale: 2
            onClicked:
            {
                remoteController.contextMenu()
                commandFeedback.start();
            }
        }
    }


    MouseArea {
        id:mArea
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top:theRow.bottom
        property int mouseStartX;
        property int mouseStartY;
        property double eventTimeStart;
        preventStealing: true;

        onPressed: {
            mouseStartX = mouse.x;
            mouseStartY = mouse.y;
            eventTimeStart = Date.now();
            //                console.log(eventTimeStart)
            mouse.accepted = true;
            runningCount = 0;
            timer.interval = 400;
            timer.start()

        }
        onReleased: {
            var mouseEndX = mouse.x;
            var mouseEndY = mouse.y;
            var eventTimeEnd = Date.now();
            var diffX = mouseEndX - mouseStartX;
            var diffY = mouseEndY - mouseStartY;
            timer.stop();
            timer.curButton = "";
            if(diffX < -200 && diffY < -200) // back gesture
            {
                remoteController.back();
                commandFeedback.start();
                return;
            }

            if(Math.abs(diffX) < 100 && Math.abs(diffY) < 100)
            {
                if(eventTimeEnd - eventTimeStart < 200)
                {
                    // depending on click position, trigger the correct event
                    computeEvent(mArea, mouseStartX, mouseStartY);
                }
                //                    console.log(eventTimeStart, " ", eventTimeEnd, " ", eventTimeEnd - eventTimeStart);
                return;
            }
            if(eventTimeEnd - eventTimeStart > 300) // handled by timer, nothing to do
                return;
            if(Math.abs(diffX) > Math.abs(diffY))
            {
                if(diffX < 0)
                {
                    if(diffX < -300)
                    {
                        remoteController.left();
                        commandFeedback.start();
                    }
                    if(diffX < -200)
                    {
                        remoteController.left();
                        commandFeedback.start();
                    }
                    if(diffX < -100)
                    {
                        commandFeedback.start();
                        remoteController.left();
                    }
                }
                else
                {
                    if(diffX > 300)
                    {
                        remoteController.left();
                        commandFeedback.start();
                    }
                    if(diffX > 200)
                    {
                        remoteController.left();
                        commandFeedback.start();
                    }
                    if(diffX > 100)
                    {
                        remoteController.left();
                        commandFeedback.start();
                    }
                }
            }
            else
            {
                if(diffY < -300)
                {
                    remoteController.up();
                    commandFeedback.start();
                }
                if(diffY < -200)
                {
                    remoteController.up();
                    commandFeedback.start();
                }
                if(diffY < -100)
                {
                    remoteController.up();
                    commandFeedback.start();
                }
                if(diffY > 300)
                {
                    remoteController.down();
                    commandFeedback.start();
                }
                if(diffY > 200)
                {
                    remoteController.down();
                    commandFeedback.start();
                }
                if(diffY > 100)
                {
                    remoteController.down();
                    commandFeedback.start();
                }
            }
            mouse.accepted = true;
        }
    }


    Image {
        source: "image://theme/icon-m-page-up?" + (timer.curButton == "up"?Theme.highlightColor:Theme.primaryColor);
        anchors.horizontalCenter: mArea.horizontalCenter
        y: theRow.height + mArea.height * 0.30
        width:mArea.height * 0.15
        height:mArea.height * 0.15
    }
    Image {
        source: "image://theme/icon-m-page-down?" + (timer.curButton == "down"?Theme.highlightColor:Theme.primaryColor)
        anchors.horizontalCenter: mArea.horizontalCenter
        y: theRow.height + mArea.height * 0.55
        width:mArea.height * 0.15
        height:mArea.height * 0.15
    }
    Image {
        source: "image://theme/icon-m-page-up?" + (timer.curButton == "right"?Theme.highlightColor:Theme.primaryColor)
        anchors.verticalCenter: mArea.verticalCenter
        x:mArea.width / 2 - (width * 4 / 3)
        width:mArea.height * 0.15
        height:width
        rotation:270
    }
    Image {
        source: "image://theme/icon-m-page-up?" + (timer.curButton == "left"?Theme.highlightColor:Theme.primaryColor)
        anchors.verticalCenter: mArea.verticalCenter
        x:mArea.width / 2 + (width / 3)
        width:mArea.height * 0.15
        height:width
        rotation:90
    }

    function computeEvent(mouseArea, clickX, clickY)
    {
        var centerX = mouseArea.width / 2;
        var centerY = mouseArea.height / 2;
        var centerSize = mouseArea.height * 0.15;
        var startUpY = centerY - centerSize;
        var startDownY = centerY + centerSize;
        var startLeft = centerX - centerSize;
        var startRight = centerX + centerSize;
        var xCentered = (clickX > startLeft) && (clickX < startRight);
        var yCentered = (clickY > startUpY && clickY < startDownY);
        console.log("Event pressed", clickX, ",", clickY, " vs ", centerX, ",", centerY, " (", startUpY, ",", startDownY, ")")
        if(xCentered && yCentered)
        {
            commandFeedback.start();
            remoteController.select();
        }
        else if(xCentered)
        {
            if(clickY <= startUpY)
            {
                remoteController.up();
                commandFeedback.start();
            }
            else if(clickY >= startDownY)
            {
                remoteController.down();
                commandFeedback.start();
            }
        }
        else if(yCentered)
        {
            if(clickX <= startLeft)
            {
                remoteController.left();
                commandFeedback.start();
            }
            else if(clickX >= startRight)
            {
                remoteController.right();
                commandFeedback.start();
            }
        }
    }
}
