project(Kontroller CXX)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

find_package (Qt5 COMPONENTS Core Network Qml Gui Quick LinguistTools REQUIRED)

include(FindPkgConfig)
pkg_check_modules(SAILFISH sailfishapp sailfishsilica)

include(ExternalProject)

if (${SAILFISH_FOUND})
	set(BUILD_FOR_SAILFISH ON)
	message("Build for sailfish os")
	add_compile_definitions(SAILFISH_TARGET)
	set(AppName "harbour-kontroller")
	set(QML_RESOURCES_FILE)
else()
	set(BUILD_FOR_SAILFISH OFF)
	message("Building for desktop")
	find_package(Qt5 COMPONENTS QuickControls2 REQUIRED)
	set(CMAKE_AUTORCC ON)
	set(QML_RESOURCES_FILE qml_desktop/qml.qrc)
	set(AppName "kontroller")
	enable_testing()
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BUILD_SHARED_LIBS OFF)

if("${CMAKE_GENERATOR}" STREQUAL "Ninja")
	set(lib_install_command ninja install)
else()
	set(lib_install_command make install)
endif()

ExternalProject_Add(QJSONRPC
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/qjsonrpc
	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/qjsonrpc_build
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/libs_install"
	INSTALL_COMMAND DESTDIR=<INSTALL_DIR> ${lib_install_command}
	CMAKE_CACHE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
)
ExternalProject_Add(MDNS_QT
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/mdns_qt
	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/mdns_qt_build
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/libs_install"
	INSTALL_COMMAND DESTDIR=<INSTALL_DIR> ${lib_install_command}
	CMAKE_CACHE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
)
ExternalProject_Add(MINIDSPLIB
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/minidsplib
	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/minidsplib_build
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/libs_install"
	INSTALL_COMMAND DESTDIR=<INSTALL_DIR> ${lib_install_command}
	CMAKE_CACHE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
)
ExternalProject_Add(ARP
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/arp
	BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/arp_build
	DOWNLOAD_COMMAND ""
	UPDATE_COMMAND ""
	INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/libs_install"
	INSTALL_COMMAND DESTDIR=<INSTALL_DIR> ${lib_install_command}
	CMAKE_CACHE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
)

if (${BUILD_FOR_SAILFISH})
	set(SAILFISH_SPECIFIC_SOURCES kontroller/sbiconimageprovider.cpp)
else()
	set(SAILFISH_SPECIFIC_SOURCES )
endif()

FILE(GLOB TsFiles "kontroller/translations/*.ts")
set_source_files_properties(${TsFiles} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/l10n)
qt5_add_translation(QmFiles ${TsFiles})

add_custom_target(run_lupdate COMMAND
	${Qt5_LUPDATE_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/qml ${CMAKE_CURRENT_SOURCE_DIR}/qml_desktop -ts ${TsFiles}
)


add_executable(${AppName}
	src/main.cpp
	src/albuminformationservice.cpp
	src/downloadservice.cpp
	src/movieinformationservice.cpp
	src/playlistservice.cpp
	src/themeinformation.cpp
	src/applicationsettings.cpp
	src/episodeinformationservice.cpp
	src/musiccontrol.cpp
	src/remote.cpp
	src/tvshowinformationservice.cpp
	src/artistinformationservice.cpp
	src/file.cpp
	src/musicservice.cpp
	src/utils.cpp
	src/audiostream.cpp
	src/imagecontrol.cpp
	src/player.cpp
	src/seasoninformationservice.cpp
	src/videocontrol.cpp
	src/client.cpp
	src/kodiservicediscovery.cpp
	src/playerservice.cpp
	src/server.cpp
	src/videoservice.cpp
	src/deviceinformation.cpp
	src/libraryservice.cpp
	src/playinginformation.cpp
	src/subtitle.cpp
	src/downloadlocation.cpp
	src/playlistitem.cpp
	src/systemservice.cpp
	src/plugins/wakeup/wakeupplugin.cpp
	src/plugins/wakeup/wolwakeupplugin.cpp
	src/plugins/volume/kodivolumeplugin.cpp
	src/plugins/volume/minidspvolumeplugin.cpp
	src/plugins/volume/volumeplugin.cpp
	src/requests/albumsrequest.cpp
	src/requests/moviesrequest.cpp
	src/requests/playlistrequest.cpp
	src/requests/songsrequest.cpp
	src/requests/tvshowepisodesrequest.cpp
	src/requests/tvshowseasonsrequest.cpp
	src/requests/tvshowsrequest.cpp
	src/services/imageservice.cpp
	${QML_RESOURCES_FILE}
	${SAILFISH_SPECIFIC_SOURCES}
	${QmFiles}
	)

add_dependencies(${AppName} MDNS_QT QJSONRPC MINIDSPLIB ARP)

target_compile_definitions(${AppName} PRIVATE
	$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_compile_options(${AppName} PRIVATE -Wall)
target_include_directories(${AppName} PRIVATE
	$<BUILD_INTERFACE:
${SAILFISH_INCLUDE_DIRS}
>
	"${CMAKE_CURRENT_BINARY_DIR}/libs_install/usr/local/include"
)
message("inc dirs: ${SAILFISH_INCLUDE_DIRS}")
target_link_libraries(${AppName}
	Qt5::Core
	Qt5::Gui
	Qt5::Qml
	Qt5::Quick
	arp
	qjsonrpc
	mdns_qt
	minidsplib
	${SAILFISH_LDFLAGS}
)
target_link_directories(${AppName} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libs_install/usr/local/lib )

install(TARGETS ${AppName}
	RUNTIME DESTINATION bin
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/l10n/
	DESTINATION share/${AppName}/translations
#	FILES_MATCHING PATTERN "*.qm"
)
install(FILES kontroller/${AppName}.desktop
	DESTINATION share/applications
)

if (${BUILD_FOR_SAILFISH})
	install(DIRECTORY qml
		DESTINATION share/${AppName}
	)
	install(DIRECTORY kontroller/icons/sailfish/
		DESTINATION share/${AppName}/assets )
	install(FILES kontroller/icons/86x86/${AppName}.png
		DESTINATION share/icons/hicolor/86x86/apps
	)
	install(FILES kontroller/icons/108x108/${AppName}.png
		DESTINATION share/icons/hicolor/108x108/apps
	)
	install(FILES kontroller/icons/128x128/${AppName}.png
		DESTINATION share/icons/hicolor/128x128/apps
	)
	install(FILES kontroller/icons/172x172/${AppName}.png
		DESTINATION share/icons/hicolor/172x172/apps
	)
endif()
# Get the other files reachable from the project tree in Qt Creator

add_custom_target(distfiles
	SOURCES
		kontroller/${AppName}.desktop
		rpm/${AppName}.changes.in
		rpm/${AppName}.changes.run.in
		rpm/${AppName}.spec
		rpm/${AppName}.yaml
		${TsFiles})

# Tell Qt Creator where the application executable(s) would be located on the
# device.
#
# It is not necessary to list other deployables than executables (runtime
# targets) here. The deployment process of Sailfish OS projects is opaque to
# Qt Creator and the information contained in QtCreatorDeployment.txt is only
# used to locate the executable associated with the active run configuration
# on the device in order to run it.
#
# Search the Qt Creator Manual to learn about the QtCreatorDeployment.txt file
# format.
file(WRITE "${CMAKE_BINARY_DIR}/QtCreatorDeployment.txt"
    "${CMAKE_INSTALL_PREFIX}
${CMAKE_BINARY_DIR}/${AppName}:bin
")

