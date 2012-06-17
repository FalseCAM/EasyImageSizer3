cmake_minimum_required(VERSION 2.8.0)

project(OVERLAY_TEXT)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${OVERLAY_TEXT_BINARY_DIR} ${QT_INCLUDE_DIR} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(OVERLAY_TEXT_SRCS
    overlay-text.cpp
)

set(OVERLAY_TEXT_HEADERS
	overlay-text.h
)

set(OVERLAY_TEXT_UIS
    overlay-text.ui
)

QT4_WRAP_CPP(OVERLAY_TEXT_MOC_SRCS ${OVERLAY_TEXT_HEADERS})
QT4_WRAP_UI(OVERLAY_TEXT_UIS_H ${OVERLAY_TEXT_UIS})

add_library(overlay-text SHARED ${OVERLAY_TEXT_SRCS}
							${OVERLAY_TEXT_MOC_SRCS}
							${OVERLAY_TEXT_UIS_H})
target_link_libraries(overlay-text easyimagesizer3 ${QT_LIBRARIES})
