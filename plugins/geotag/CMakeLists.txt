cmake_minimum_required(VERSION 2.8.1)

project(GEOTAG)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
include_directories( ${GEOTAG_BINARY_DIR} ${QT_INCLUDE_DIR} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(QT_USE_QTNETWORK)

set(GEOTAG_SRCS
    geotag.cpp
map.cpp
)

set(GEOTAG_HEADERS
        geotag.h
map.h
)

set(GEOTAG_UIS
    geotag.ui
)

QT4_WRAP_CPP(GEOTAG_MOC_SRCS ${GEOTAG_HEADERS})
QT4_WRAP_UI(GEOTAG_UIS_H ${GEOTAG_UIS})

add_library(geotag SHARED ${GEOTAG_SRCS}
							${GEOTAG_MOC_SRCS}
							${GEOTAG_UIS_H})
target_link_libraries(geotag easyimagesizer3 exiv2 ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARY})
