cmake_minimum_required(VERSION 3.10)

project(arp CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core REQUIRED)

set(CMAKE_AUTOMOC ON)

set(sources
	"${CMAKE_CURRENT_SOURCE_DIR}/macaddressfinder.cpp"
)

set(headers
	"${CMAKE_CURRENT_SOURCE_DIR}/macaddressfinder.h"
)

add_library(arp ${sources} ${headers})
target_link_libraries(arp Qt5::Core)

install(TARGETS arp DESTINATION lib)
install(FILES ${headers} DESTINATION include/arp)
