import QtQuick 2.0

QtObject {
    property var informationPageComponents

    function createMusicPage(file) {
        if(!!informationPageComponents[file.filetype])
        {
            createInformationPage(file);
            return;
        }
        var musicList = pageStack.push("MusicPage.qml", {
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

            newView = component.createObject(null, data);
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
        var videoList = pageStack.push("VideoPage.qml", {
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
    }

    function createImagePage(file) {
        var imageList = pageStack.push("ImagePage.qml", {
                                           "visible": true,
                                           "browsingMode": file.filetype,
                                           "browsingValue": file.file,
                                           "label":file.label
                                       });
        imageList.mediaClicked.connect(createImagePage);
        imageList.mediaInformationClicked.connect(createInformationPage);
        imageList.remoteClicked.connect(pushRemotePage);
        imageList.currentClicked.connect(pushCurrentPage);
        imageList.backToMenuClicked.connect(toMenu);
    }

    function createCurrentlyPlayingPage()
    {
        var page = pageStack.push("CurrentlyPlaying.qml");
        page.mediaInformationClicked.connect(function (file)
        {
            createInformationPage(file);
        });
    }
}
