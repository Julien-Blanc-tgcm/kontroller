#include <QApplication>
#include <QtQml>
#include "musicservice.h"
#include <iostream>
#include <cassert>
#include "kodiremote.h"
#include "kodisettings.h"
#include "kodiclient.h"
#include "kodiplayercontrol.h"
#include "statusservice.h"
#include "kodiplayer.h"
#include "playlistcontrol.h"
#include "playlistitem.h"
#include "videoservice.h"
#include "kodisettingsmanager.h"
#include <QScreen>
#include "deviceinformation.h"
#include "artistinformationservice.h"
#include "albuminformationservice.h"
#include "musiccontrol.h"
#include "videocontrol.h"
#include "movieinformationservice.h"
#include "tvshowinformationservice.h"
#include "seasoninformationservice.h"
#include "episodeinformationservice.h"
#include "kodiplayinginformation.h"

void registerTypes()
{
    int ret = qmlRegisterType<KodiFile>();
    assert(ret);
    ret = qmlRegisterType<MusicService>("eu.tgcm", 1, 0, "MusicService");
    assert(ret);
    ret = qmlRegisterType<KodiRemote>("eu.tgcm", 1, 0, "KodiRemote");
    assert(ret);
    ret = qmlRegisterType<KodiSettings>("eu.tgcm", 1, 0, "KodiSettings");
    assert(ret);
    ret = qmlRegisterType<KodiPlayerControl>("eu.tgcm", 1, 0, "KodiPlayerControl");
    assert(ret);
    ret = qmlRegisterType<StatusService>("eu.tgcm", 1, 0, "StatusService");
    assert(ret);
    ret = qmlRegisterType<KodiPlayer>();
    assert(ret);
    ret = qmlRegisterType<PlaylistControl>("eu.tgcm", 1, 0, "PlaylistControl");
    assert(ret);
    ret = qmlRegisterType<PlaylistItem>();
    assert(ret);
    ret = qmlRegisterType<VideoService>("eu.tgcm", 1, 0, "VideoService");
    assert(ret);
    ret = qmlRegisterType<DeviceInformation>("eu.tgcm", 1, 0, "DeviceInformation");
    assert(ret);
    ret = qmlRegisterType<ArtistInformationService>("eu.tgcm", 1, 0, "ArtistInformationService");
    assert(ret);
    ret = qmlRegisterType<AlbumInformationService>("eu.tgcm", 1, 0, "AlbumInformationService");
    assert(ret);
    ret = qmlRegisterType<MusicControl>("eu.tgcm", 1, 0, "MusicControl");
    assert(ret);
    ret = qmlRegisterType<VideoControl>("eu.tgcm", 1, 0, "VideoControl");
    assert(ret);
    ret = qmlRegisterType<MovieInformationService>("eu.tgcm", 1, 0, "MovieInformationService");
    assert(ret);
    ret = qmlRegisterType<TvShowInformationService>("eu.tgcm", 1, 0, "TvShowInformationService");
    assert(ret);
    ret = qmlRegisterType<SeasonInformationService>("eu.tgcm", 1, 0, "SeasonInformationService");
    assert(ret);
    ret = qmlRegisterType<EpisodeInformationService>("eu.tgcm", 1, 0, "EpisodeInformationService");
    assert(ret);
    ret = qmlRegisterType<KodiPlayingInformation>("eu.tgcm", 1, 0, "KodiPlayingInformation");
    assert(ret);
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //QJsonRpcHttpClient* client = new QJsonRpcHttpClient("http://localhost:8080/jsonrpc",&app);
    registerTypes();
    KodiSettings settings;
    KodiClient::current().setServerAddress(settings.serverAddress());
    KodiClient::current().setServerPort(settings.serverPort());
    KodiClient::current().refresh();
/*    MusicService service("/mnt/files/mp3/MODERNE/Muse");
    service.refresh();
    for(int i = 0; i < service.files().size(); ++i)
    {
        std::cout << service.files().at(i)->file().toUtf8().constData() << " " <<
                     service.files().at(i)->filetype().toUtf8().constData() << "\n";
    }
    std::cout << std::endl; */
   // service.playFile(service.files().back());
    DeviceInformation inf;
    inf.setup(app);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    return app.exec();
}
