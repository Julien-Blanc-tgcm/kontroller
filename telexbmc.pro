TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    musicservice.cpp \
    kodifile.cpp \
    kodiclient.cpp \
    kodiremote.cpp \
    kodisettings.cpp \
    kodiplayercontrol.cpp \
    kodisettingsmanager.cpp \
    statusservice.cpp \
    kodiplayer.cpp \
    playlistservice.cpp \
    videoservice.cpp \
    kodiplayerservice.cpp \
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
    kodiplayinginformation.cpp \
    playlistcontrol.cpp \
    playlistitem.cpp

RESOURCES += qml/qml.qrc \
    icons.qrc

INCLUDEPATH += ../qjsonrpc/src

QMAKE_CXXFLAGS = --std=c++11 -Wall

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

equals(ANDROID_TARGET_ARCH, armeabi-v7a) {
  LIBS += -L../qjsonrpc-Android_pour_armeabi_v7a_GCC_4_8_Qt_5_2_1-Release/src
  LIBS += -L../qjsonrpc-Android_pour_armeabi_v7a_GCC_4_8_Qt_5_5_1-Release/src
} else {
  LIBS += -L../qjsonrpc-Desktop_Qt_5_2_1_GCC_64bit-Debug/src
  LIBS += -L../qjsonrpc-Qt5_desktop-Release/src
}

#LIBS += -L../build-qjsonrpc-Android_pour_armeabi_v7a_GCC_4_8_Qt_5_2_1-Debug/src
LIBS += -lqjsonrpc

HEADERS += \
    musicservice.h \
    kodifile.h \
    kodiclient.h \
    kodiremote.h \
    kodisettings.h \
    kodiplayercontrol.h \
    kodisettingsmanager.h \
    statusservice.h \
    kodiplayer.h \
    playlistservice.h \
    videoservice.h \
    kodiplayerservice.h \
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
    kodiplayinginformation.h \
    playlistcontrol.h \
    playlistitem.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
#    ANDROID_EXTRA_LIBS = ../qjsonrpc-Android_pour_armeabi_v7a_GCC_4_8_Qt_5_5_1-Release/src/libqjsonrpc.so
    ANDROID_EXTRA_LIBS = ../qjsonrpc-android/src/libqjsonrpc.so
}

DISTFILES += \
    qml/qmldir

