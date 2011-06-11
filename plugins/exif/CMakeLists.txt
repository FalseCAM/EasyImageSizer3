cmake_minimum_required(VERSION 2.8.4)

project(EXIF)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(EXIF_SRCS
    exif.cpp
)

set(EXIF_HEADERS
	exif.h
)

set(EXIF_UIS
    exif.ui
)

QT4_WRAP_CPP(EXIF_MOC_SRCS ${EXIF_HEADERS})
QT4_WRAP_UI(EXIF_UIS_H ${EXIF_UIS})

add_library(exif SHARED ${EXIF_SRCS}
							${EXIF_MOC_SRCS}
							${EXIF_UIS_H})
target_link_libraries(exif easyimagesizer3 exiv2 ${QT_LIBRARIES})