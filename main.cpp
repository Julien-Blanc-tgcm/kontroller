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
#include "playlistservice.h"
#include "kodiplaylistitem.h"
#include "videoservice.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    //QJsonRpcHttpClient* client = new QJsonRpcHttpClient("http://localhost:8080/jsonrpc",&app);
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
    ret = qmlRegisterType<PlaylistService>("eu.tgcm", 1, 0, "PlaylistService");
    assert(ret);
    ret = qmlRegisterType<KodiPlaylistItem>();
    assert(ret);
    ret = qmlRegisterType<VideoService>("eu.tgcm", 1, 0, "VideoService");
    assert(ret);
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
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
