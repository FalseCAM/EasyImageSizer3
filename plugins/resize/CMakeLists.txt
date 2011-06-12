cmake_minimum_required(VERSION 2.8.4)

project(RESIZE)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${RESIZE_BINARY_DIR} ${QT_INCLUDE_DIR} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(RESIZE_SRCS
    resize.cpp
)

set(RESIZE_HEADERS
	resize.h
)

set(RESIZE_UIS
    resize.ui
)

QT4_WRAP_CPP(RESIZE_MOC_SRCS ${RESIZE_HEADERS})
QT4_WRAP_UI(RESIZE_UIS_H ${RESIZE_UIS})

add_library(resize SHARED ${RESIZE_SRCS}
							${RESIZE_MOC_SRCS}
							${RESIZE_UIS_H})
target_link_libraries(resize easyimagesizer3 ${QT_LIBRARIES})