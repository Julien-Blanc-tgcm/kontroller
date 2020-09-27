import QtQuick 2.0

QtObject {
    property var musicPageComponent:null
    property var videoPageComponent:null
    property var informationPageComponents
    property var currentlyPlayingPageComponent: null

    function createMusicPage(file) {
        if(!!informationPageComponents[file.filetype])
        {
            createInformationPage(file);
            return;
        }
        if(!musicPageComponent)
            musicPageComponent = Qt.createComponent("MusicPage.qml");
        if(musicPageComponent.status !== Component.Ready)
        {
            console.log("Error at component creation");
            console.log(musicPageComponent.errorString());
            return;
        }
        var musicList = musicPageComponent.createObject(pageStack,
            {
                visible: true,
                browsingMode: file.filetype,
                browsingValue:file.file,
                label:file.label
            });
        musicList.mediaClicked.connect(createMusicPage);
        musicList.mediaInformationClicked.connect(createInformationPage);
        musicList.remoteClicked.connect(pushRemotePage);
        musicList.currentClicked.connect(pushCurrentPage);
        musicList.backToMenuClicked.connect(toMenu);
        pageStack.push(musicList);
    }

    function createInformationPage(file, creationData)
    {
        var newView;
        var component = internal.informationPageComponents[file.filetype];
        if(component && component.status === Component.Ready)
        {
            console.log("Create page for item ", file.id, " of type ", file.filetype);
            var data = {"itemId":file.id};
            if(creationData !== null)
            {
                // extradata is needed for tv show informations (need both the tv show and the season parameter)
                for(var key in creationData)
                    data[key] = creationData[key]; // copy data to creation parameters
            }

            newView = component.createObject(pageStack, data);
            if(newView)
            {
                if(typeof(newView.mediaInformationClicked) !== "undefined")
                    newView.mediaInformationClicked.connect(createInformationPage);
                if(typeof(newView.remoteClicked) !== "undefined")
                    newView.remoteClicked.connect(pushRemotePage);
                if(typeof(newView.currentClicked) !== "undefined")
                    newView.currentClicked.connect(pushCurrentPage);
                if(typeof(newView.backToMenuClicked) !== "undefined")
                    newView.backToMenuClicked.connect(toMenu);
                //newView.label = pageStack.currentItem.label + "/" + label;
                newView.label = file.label;
            }
            else
                console.log("Error at view creation : " + component.errorString())
        }
        else if(component)
        {
            console.log(file.filetype + " " + component.errorString());
        }
        else
        {
            console.log("no component defined for " + file.filetype);
        }
        if(newView)
            pageStack.push(newView);
    }

    function createVideoPage(file) {
        if(!!informationPageComponents[file.filetype])
        {
            createInformationPage(file);
            return;
        }
        if(!videoPageComponent)
            videoPageComponent = Qt.createComponent("VideoPage.qml");
        if(videoPageComponent.status !== Component.Ready)
        {
            console.log("Error at video component creation");
            console.log(videoPageComponent.errorString());
            return;
        }
        var videoList = videoPageComponent.createObject(pageStack,
            {
                "visible":true,
                "browsingMode":file.filetype,
                "browsingValue":file.file,
                "label":file.label
            });
        videoList.mediaClicked.connect(createVideoPage);
        videoList.mediaInformationClicked.connect(createInformationPage);
        videoList.remoteClicked.connect(pushRemotePage);
        videoList.currentClicked.connect(pushCurrentPage);
        videoList.backToMenuClicked.connect(toMenu);
        pageStack.push(videoList);
    }

    function createCurrentlyPlayingPage()
    {
        if(!currentlyPlayingPageComponent)
            currentlyPlayingPageComponent = Qt.createComponent("CurrentlyPlaying.qml");
        if(currentlyPlayingPageComponent.status !== Component.Ready)
        {
            console.log("Error at CurrentlyPlaying component creation");
            console.log(videoPageComponent.errorString());
            return;
        }
        var page = currentlyPlayingPageComponent.createObject(pageStack);
        page.mediaInformationClicked.connect(function (file)
        {
            internal.createInformationPage(file);
        });
        pageStack.push(page);
    }
}
