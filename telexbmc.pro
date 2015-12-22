TEMPLATE = app

QT += qml quick
QT += widgets

#CONFIG += sailfishapp

SOURCES += main.cpp \
    musicservice.cpp \
    statusservice.cpp \
    playlistservice.cpp \
    videoservice.cpp \
    deviceinformation.cpp \
    artistinformationservice.cpp \
    albumsrequest.cpp \
    songsrequest.cpp \
    albuminformationservice.cpp \
    musiccontrol.cpp \
    videocontrol.cpp \
    movieinformationservice.cpp \
    tvshowinformationservice.cpp \
    tvshowseasonsrequest.cpp \
    seasoninformationservice.cpp \
    tvshowepisodesrequest.cpp \
    utils.cpp \
    episodeinformationservice.cpp \
    playlistcontrol.cpp \
    playlistitem.cpp \
    playercontrol.cpp \
    subtitle.cpp \
    client.cpp \
    playinginformation.cpp \
    file.cpp \
    player.cpp \
    playerservice.cpp \
    settingsmanager.cpp \
    settings.cpp \
    remote.cpp

RESOURCES += qml/qml.qrc \
    icons.qrc

INCLUDEPATH += ../qjsonrpc/src

QMAKE_CXXFLAGS = --std=c++11 -Wall

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

#MER_TARGET=SailfishOS-armv7hl

equals(ANDROID_TARGET_ARCH, armeabi-v7a) {
  LIBS += -L../qjsonrpc-Android_pour_armeabi_v7a_GCC_4_8_Qt_5_2_1-Release/src
  LIBS += -L../qjsonrpc-Android_pour_armeabi_v7a_GCC_4_8_Qt_5_5_1-Release/src
} else {
  equals(MER_TARGET,SailfishOS-armv7hl) {
    LIBS += -L../qjsonrpc-Mer/src
  } else {
  LIBS += -L../qjsonrpc-Desktop_Qt_5_2_1_GCC_64bit-Debug/src
  LIBS += -L../qjsonrpc-Qt5_desktop-Release/src
  }
}

CONFIG(sailfishapp) {
  DEFINES+= SAILFISH_TARGET
}

#LIBS += -L../build-qjsonrpc-Android_pour_armeabi_v7a_GCC_4_8_Qt_5_2_1-Debug/src
LIBS += -lqjsonrpc

HEADERS += \
    musicservice.h \
    statusservice.h \
    playlistservice.h \
    videoservice.h \
    deviceinformation.h \
    artistinformationservice.h \
    utils.h \
    albumsrequest.h \
    songsrequest.h \
    albuminformationservice.h \
    musiccontrol.h \
    videocontrol.h \
    movieinformationservice.h \
    tvshowinformationservice.h \
    tvshowseasonsrequest.h \
    seasoninformationservice.h \
    tvshowepisodesrequest.h \
    episodeinformationservice.h \
    playlistcontrol.h \
    playlistitem.h \
    playercontrol.h \
    subtitle.h \
    client.h \
    playinginformation.h \
    file.h \
    player.h \
    playerservice.h \
    settingsmanager.h \
    settings.h \
    remote.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
#    ANDROID_EXTRA_LIBS = ../qjsonrpc-Android_pour_armeabi_v7a_GCC_4_8_Qt_5_5_1-Release/src/libqjsonrpc.so
    ANDROID_EXTRA_LIBS = ../qjsonrpc-android/src/libqjsonrpc.so
}

DISTFILES += \
    qml/qmldir \
    telexbmc.prf \
    rpm/telexbmc.yaml \
    telexbmc.desktop

equals(MER_TARGET,SailfishOS-armv7hl) {
message(deploying lib)
lib.path = /usr/share/telexbmc/lib/
lib.files += ../qjsonrpc-Mer/src/libqjsonrpc.so.1
INSTALLS += lib
}
