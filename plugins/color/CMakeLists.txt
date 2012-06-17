cmake_minimum_required(VERSION 2.8.0)

project(COLOR)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${COLOR_BINARY_DIR} ${QT_INCLUDE_DIR} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(COLOR_SRCS
    color.cpp
)

set(COLOR_HEADERS
        color.h
)

set(COLOR_UIS
    color.ui
)

set(COLOR_RCS
       color.qrc
)

QT4_WRAP_CPP(COLOR_MOC_SRCS ${COLOR_HEADERS})
QT4_ADD_RESOURCES( COLOR_RC_SRCS ${COLOR_RCS} )
QT4_WRAP_UI(COLOR_UIS_H ${COLOR_UIS})

add_library(color SHARED ${COLOR_SRCS}
                                ${COLOR_MOC_SRCS}
                                ${COLOR_RC_SRCS}
                                ${COLOR_UIS_H})
target_link_libraries(color easyimagesizer3 ${QT_LIBRARIES})
