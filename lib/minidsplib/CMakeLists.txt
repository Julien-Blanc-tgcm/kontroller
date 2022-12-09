cmake_minimum_required(VERSION 3.10)

project(minidsplib CXX)
set(CMAKE_CXX_STANDARD 17)

set(sources
	"${CMAKE_CURRENT_SOURCE_DIR}/query.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/reply.cpp"
)

set(headers
	"${CMAKE_CURRENT_SOURCE_DIR}/constants.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/query.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/reply.h"
)

add_library(minidsplib ${sources} ${headers})

install(TARGETS minidsplib DESTINATION lib)
install(FILES ${headers} DESTINATION include/minidsplib)
