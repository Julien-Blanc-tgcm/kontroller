#ifdef SAILFISH_TARGET
#include <sailfishapp.h>
#include <QGuiApplication>
#include <QQuickView>
#include <QQuickItem>
#else
#include <QApplication>
#endif
#include <QtQml>
#include "musicservice.h"
#include <cassert>
#include "downloadservice.h"
#include "remote.h"
#include "settings.h"
#include "client.h"
#include "playercontrol.h"
#include "statusservice.h"
#include "player.h"
#include "playlistcontrol.h"
#include "playlistitem.h"
#include "videoservice.h"
#include "settingsmanager.h"
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
#include "playinginformation.h"
#include "server.h"
#ifndef SAILFISH_TARGET
#include "themeinformation.h"
#endif

void registerTypes()
{
    using namespace eu::tgcm::kontroller;
#ifdef SAILFISH_TARGET
    auto qmlprefix = "harbour.eu.tgcm";
#else
    auto qmlprefix = "eu.tgcm";
#endif
    int ret = qmlRegisterType<File>();
    assert(ret);
    ret = qmlRegisterType<Subtitle>();
    assert(ret);
    ret = qmlRegisterType<AudioStream>();
    assert(ret);
    ret = qmlRegisterType<Server>();
    assert(ret);
    ret = qmlRegisterType<MusicService>(qmlprefix, 1, 0, "MusicService");
    assert(ret);
    ret = qmlRegisterType<Remote>(qmlprefix, 1, 0, "Remote");
    assert(ret);
    ret = qmlRegisterType<Settings>(qmlprefix, 1, 0, "Settings");
    assert(ret);
    ret = qmlRegisterType<PlayerControl>(qmlprefix, 1, 0, "PlayerControl");
    assert(ret);
    ret = qmlRegisterType<StatusService>(qmlprefix, 1, 0, "StatusService");
    assert(ret);
    ret = qmlRegisterType<Player>();
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
    ret = qmlRegisterType<PlayingInformation>(qmlprefix, 1, 0, "PlayingInformation");
    assert(ret);
    ret = qmlRegisterType<DownloadService>(qmlprefix, 1, 0, "DownloadService");
#ifndef SAILFISH_TARGET
    ret = qmlRegisterType<ThemeInformation>(qmlprefix, 1, 0, "ThemeInformation");
    assert(ret);
#endif
}


int main(int argc, char *argv[])
{
#ifdef SAILFISH_TARGET
	QGuiApplication* app = SailfishApp::application(argc, argv);
	auto view = SailfishApp::createView();
	registerTypes();
	view->setSource(SailfishApp::pathTo("qml/sailfish/kontroller.qml"));
	view->showFullScreen();
	auto engine = QtQml::qmlEngine(view->rootObject());
	QObject::connect(engine, &QQmlEngine::quit, app, &QGuiApplication::quit);
#else
	QApplication* app = new QApplication(argc, argv);
	QQmlApplicationEngine engine;
	registerTypes();

	eu::tgcm::kontroller::DeviceInformation inf;
	inf.setup(*app);

	//QQmlComponent component(&engine);
	engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
#endif

	eu::tgcm::kontroller::Client::current().refresh();

	return app->exec();
}
