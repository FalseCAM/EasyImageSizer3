cmake_minimum_required(VERSION 2.8.4)

project(RENAME)

find_package("Qt4" REQUIRED)
include(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
# enable warnings
add_definitions( -Wall )
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)

set( LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/plugins" )

set(RENAME_SRCS
    rename.cpp
)

set(RENAME_HEADERS
	rename.h
)

set(RENAME_UIS
    rename.ui
)

QT4_WRAP_CPP(RENAME_MOC_SRCS ${RENAME_HEADERS})
QT4_WRAP_UI(RENAME_UIS_H ${RENAME_UIS})

add_library(rename SHARED ${RENAME_SRCS}
							${RENAME_MOC_SRCS}
							${RENAME_UIS_H})
target_link_libraries(rename easyimagesizer3 ${QT_LIBRARIES})