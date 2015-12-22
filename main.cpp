#ifdef SAILFISH_TARGET
#include <sailfishapp.h>
#include <QGuiApplication>
#else
#include <QApplication>
#endif
#include <QtQml>
#include "musicservice.h"
#include <cassert>
#include "kodiremote.h"
#include "kodisettings.h"
#include "client.h"
#include "playercontrol.h"
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
#ifdef SAILFISH_TARGET
    auto qmlprefix = "harbour.eu.tgcm";
#else
    auto qmlprefix = "eu.tgcm";
#endif
    int ret = qmlRegisterType<KodiFile>();
    assert(ret);
    ret = qmlRegisterType<Subtitle>();
    assert(ret);
    ret = qmlRegisterType<MusicService>(qmlprefix, 1, 0, "MusicService");
    assert(ret);
    ret = qmlRegisterType<KodiRemote>(qmlprefix, 1, 0, "KodiRemote");
    assert(ret);
    ret = qmlRegisterType<KodiSettings>(qmlprefix, 1, 0, "KodiSettings");
    assert(ret);
    ret = qmlRegisterType<PlayerControl>(qmlprefix, 1, 0, "PlayerControl");
    assert(ret);
    ret = qmlRegisterType<StatusService>(qmlprefix, 1, 0, "StatusService");
    assert(ret);
    ret = qmlRegisterType<KodiPlayer>();
    assert(ret);
    ret = qmlRegisterType<PlaylistControl>(qmlprefix, 1, 0, "PlaylistControl");
    assert(ret);
    ret = qmlRegisterType<PlaylistItem>();
    assert(ret);
    ret = qmlRegisterType<VideoService>(qmlprefix, 1, 0, "VideoService");
    assert(ret);
    ret = qmlRegisterType<DeviceInformation>(qmlprefix, 1, 0, "DeviceInformation");
    assert(ret);
    ret = qmlRegisterType<ArtistInformationService>(qmlprefix, 1, 0, "ArtistInformationService");
    assert(ret);
    ret = qmlRegisterType<AlbumInformationService>(qmlprefix, 1, 0, "AlbumInformationService");
    assert(ret);
    ret = qmlRegisterType<MusicControl>(qmlprefix, 1, 0, "MusicControl");
    assert(ret);
    ret = qmlRegisterType<VideoControl>(qmlprefix, 1, 0, "VideoControl");
    assert(ret);
    ret = qmlRegisterType<MovieInformationService>(qmlprefix, 1, 0, "MovieInformationService");
    assert(ret);
    ret = qmlRegisterType<TvShowInformationService>(qmlprefix, 1, 0, "TvShowInformationService");
    assert(ret);
    ret = qmlRegisterType<SeasonInformationService>(qmlprefix, 1, 0, "SeasonInformationService");
    assert(ret);
    ret = qmlRegisterType<EpisodeInformationService>(qmlprefix, 1, 0, "EpisodeInformationService");
    assert(ret);
    ret = qmlRegisterType<KodiPlayingInformation>(qmlprefix, 1, 0, "KodiPlayingInformation");
    assert(ret);
}


int main(int argc, char *argv[])
{
#ifdef SAILFISH_TARGET
    QGuiApplication* app = SailfishApp::application(argc, argv);
#else
    QApplication* app = new QApplication(argc, argv);
#endif

    QQmlApplicationEngine engine;

    //QJsonRpcHttpClient* client = new QJsonRpcHttpClient("http://localhost:8080/jsonrpc",&app);
    registerTypes();
    KodiSettings settings;
    Client::current().setServerAddress(settings.serverAddress());
    Client::current().setServerPort(settings.serverPort());
    Client::current().refresh();
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
    inf.setup(*app);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    return app->exec();
}
