import QtQuick 2.0
import QtQuick.Controls 1.4
import eu.tgcm 1.0
import "."

Rectangle
{
    color:"#000";
    Image {
        id:upbtnimage
        source:"icons/updir.png"
        height:20 * touchScalingFactor
        width:height
        anchors.left:parent.left
        anchors.top: parent.top
        MouseArea {
            anchors.fill: parent
            onClicked: if(stackedview.depth > 1) popCurrentView();
        }
        visible: stackedview.depth > 1
    }

    Text {
        id: upbtn
        text: qsTr("Sources")
        MouseArea {
            id:area
            onClicked: if(stackedview.depth > 1)
                       popCurrentView();
            anchors.fill: parent
        }
        color: Styling.linkColor
        font.bold: true
        anchors.left: upbtnimage.right
        anchors.leftMargin: 5 * scalingFactor
        anchors.verticalCenter: refreshbtn.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: refreshbtn.width + 5 * scalingFactor
        font.pixelSize: theme.fontSizeSmall
        elide:Text.ElideLeft
        clip:true
    }

    Image {
        id:refreshbtn
        height: 20 * touchScalingFactor
        width:height
        source: "icons/refresh.png"
        anchors.right: parent.right
        anchors.top:parent.top
        anchors.topMargin: 5
        anchors.rightMargin: 10
        MouseArea {
            anchors.fill: parent
            onClicked: stackedview.currentItem.refresh()
        }
    }
    StackView
    {
        anchors.top: refreshbtn.bottom
        anchors.topMargin: 5 * scalingFactor
        anchors.bottom:parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        id: stackedview
        /*initialItem: MusicPage {
            browsingMode: ""
            browsingValue: ""
            label: ""
        }*/
        Component.onCompleted: {
            if(internal.pageComponent.status === Component.Ready)
                internal.createConnections()
            else
                internal.pageComponent.statusChanged.connect(internal.createConnections());
        }
    }


    Keys.onReleased: {
        if(event.key === Qt.Key_Back || event.key === Qt.Key_Backspace)
        {
            popCurrentView();
            event.accepted = true
        }
    }
    focus:true

    function pushNewPage(filetype, file, label)
    {
        if(filetype === "")
        {
            return;
        }
        upbtn.text = stackedview.currentItem.label + "/" + label;
        for(var i = 0; i < defaultsToInformation.length; ++i)
        {
            if(defaultsToInformation[i] === filetype)
                return pushNewInformationPage(filetype, file, label);
        }

        //pushNewInformationPage_(filetype, file, label)
        if(internal.pageComponent.status === Component.Ready)
        {
            pushNewPage_(filetype, file, label);
        }
    }

    function pushNewPage_(filetype, file, label)
    {
        var newView = internal.pageComponent.createObject(stackedview, {
                                                 browsingMode: filetype,
                                                 browsingValue: file,
                                                 label:stackedview.currentItem.label + "/" + label
                                             });
        newView.mediaClicked.connect(pushNewPage)
        newView.mediaInformationClicked.connect(pushNewInformationPage);
        stackedview.push(newView)
    }

    function pushNewInformationPage(filetype, file, label)
    {
        if(filetype === "")
        {
            console.log("error")
            return;
        }
        pushNewInformationPage_(filetype, file, label)
    }

    function pushNewInformationPage_(filetype, file, label)
    {
        var newView;
        var component = internal.informationPageComponents[filetype];
        if(component && component.status === Component.Ready)
        {
            newView = component.createObject(stackedview, {"itemId":file});
            newView.mediaInformationClicked.connect(pushNewInformationPage);
            upbtn.text = stackedview.currentItem.label + "/" + label;
            newView.label = upbtn.text;
        }
        else if(component)
            console.log(filetype + ' ' + component.errorString());
        else
            console.log('No component defined for ' + filetype)

        if(newView)
            stackedview.push(newView);
    }

    function popCurrentView()
    {
        stackedview.pop()
        if(stackedview.depth > 1)
            upbtn.text = stackedview.get(stackedview.depth - 1).label
        else
            upbtn.text = "Sources";
    }

    QtObject {
        id:internal
        property var pageComponent
        property var informationPageComponents
        function createConnections()
        {
            if(internal.pageComponent.status !== Component.Ready)
                console.log(internal.pageComponent.errorString)
            var item = internal.pageComponent.createObject(stackedview, {});
            item.mediaClicked.connect(pushNewPage)
            item.mediaInformationClicked.connect(pushNewInformationPage)
            stackedview.push(item)
        }
    }

    property string pageComponent;
    /**
      * \brief stores the component to use for a given name
      * pass something like [{'type':'artist','component':'ArtistInformationPage'}]
      */
    property var informationPageComponents;
    /**
      * \brief List of types for which a click defaults to information page
      * instead of standard list page
      */
    property var defaultsToInformation

    Component.onCompleted: {
        internal.pageComponent = Qt.createComponent(pageComponent + ".qml")
        internal.informationPageComponents = {}
        for(var i = 0; i < informationPageComponents.length; ++i)
        {
            internal.informationPageComponents[informationPageComponents[i].type] =
                    Qt.createComponent(informationPageComponents[i].component + '.qml');
        }
    }
}



