import QtQuick 2.0
import harbour.eu.tgcm 1.0
import Sailfish.Silica 1.0
import "."

Page {
    id:page;
    Remote {
        id: remoteController
    }
    property int buttonWidth : 100;

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
            console.log(diffX);
            console.log(diffY);
            console.log(curButton)

            if(curButton == "left")
                remoteController.left();
            else if(curButton == "right")
                remoteController.right();
            else if(curButton == "up")
                remoteController.up();
            else if(curButton == "down")
                remoteController.down();
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
            onClicked: remoteController.back()
        }
        IconButton {
            width: buttonWidth
            height: buttonWidth
            icon.source: "image://theme/icon-m-home"
            icon.scale: 2
            onClicked: remoteController.home()
        }
        IconButton {
            width: buttonWidth
            height: buttonWidth
            icon.source: "image://theme/icon-m-question"
            icon.scale: 2
            onClicked:remoteController.info()
        }
        IconButton{
            width: buttonWidth
            height: buttonWidth
            icon.source: "image://theme/icon-m-menu"
            icon.scale: 2
            onClicked:remoteController.contextMenu()
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
            console.log(eventTimeStart)
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
                return;
            }

            if(Math.abs(diffX) < 100 && Math.abs(diffY) < 100)
            {
                if(eventTimeEnd - eventTimeStart < 200)
                {
                    // depending on click position, trigger the correct event
                    computeEvent(mArea, mouseStartX, mouseStartY);
                }
                console.log(eventTimeStart);
                console.log(eventTimeEnd);
                console.log(eventTimeEnd - eventTimeStart);
                return;
            }
            if(eventTimeEnd - eventTimeStart > 300) // handled by timer, nothing to do
                return;
            if(Math.abs(diffX) > Math.abs(diffY))
            {
                if(diffX < 0)
                {
                    if(diffX < -300)
                        remoteController.left();
                    if(diffX < -200)
                        remoteController.left();
                    if(diffX < -100)
                        remoteController.left();
                }
                else
                {
                    if(diffX > 300)
                        remoteController.left();
                    if(diffX > 200)
                        remoteController.left();
                    if(diffX > 100)
                        remoteController.left()
                }
            }
            else
            {
                if(diffY < -300)
                    remoteController.up();
                if(diffY < -200)
                    remoteController.up();
                if(diffY < -100)
                    remoteController.up();
                if(diffY > 300)
                    remoteController.down();
                if(diffY > 200)
                    remoteController.down();
                if(diffY > 100)
                    remoteController.down();
            }
            mouse.accepted = true;
        }

        Image {
            source: "image://theme/icon-m-page-up?" + (timer.curButton == "up"?Theme.highlightColor:Theme.primaryColor);
            anchors.horizontalCenter: parent.horizontalCenter
            y: parent.height / 2 - height - 30
            width:150
            height:150
        }
        Image {
            source: "image://theme/icon-m-page-down?" + (timer.curButton == "down"?Theme.highlightColor:Theme.primaryColor)
            anchors.horizontalCenter: parent.horizontalCenter
            y: parent.height / 2 + 30
            width:150
            height:150
        }
        Image {
            source: "image://theme/icon-m-page-up?" + (timer.curButton == "right"?Theme.highlightColor:Theme.primaryColor)
            anchors.verticalCenter: parent.verticalCenter
            x:parent.width / 2 + 30
            width:150
            height:150
            rotation:90
        }
        Image {
            source: "image://theme/icon-m-page-up?" + (timer.curButton == "left"?Theme.highlightColor:Theme.primaryColor)
            anchors.verticalCenter: parent.verticalCenter
            x:parent.width / 2 - width - 30
            width:150
            height:150
            rotation:270
        }
    }

    function computeEvent(mouseArea, clickX, clickY)
    {
        var centerX = (mouseArea.x + mouseArea.width) / 2;
        var centerY = (mouseArea.y + mouseArea.height) / 2;
        var xCentered = (clickX > (centerX * 0.8) && clickX < (centerX * 1.2));
        var yCentered = (clickY > (centerY * 0.8) && clickY < (centerY * 1.2));
        if(xCentered && yCentered)
            remoteController.select();
        else if(xCentered)
        {
            if(clickY <= centerY * 0.8)
                remoteController.up();
            else if(clickY >= centerY * 1.2)
                remoteController.down();
        }
        else if(yCentered)
        {
            if(clickX <= centerX * 0.8)
                remoteController.left()
            else
                remoteController.right();
        }
    }
}
