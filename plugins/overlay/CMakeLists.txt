cmake_minimum_required(VERSION 2.8.1)

project(OVERLAY)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${OVERLAY_BINARY_DIR} ${QT_INCLUDE_DIR} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(OVERLAY_SRCS
    overlay.cpp
)

set(OVERLAY_HEADERS
	overlay.h
)

set(OVERLAY_UIS
    overlay.ui
)

QT4_WRAP_CPP(OVERLAY_MOC_SRCS ${OVERLAY_HEADERS})
QT4_WRAP_UI(OVERLAY_UIS_H ${OVERLAY_UIS})

add_library(overlay SHARED ${OVERLAY_SRCS}
							${OVERLAY_MOC_SRCS}
							${OVERLAY_UIS_H})
target_link_libraries(overlay easyimagesizer3 ${QT_LIBRARIES})